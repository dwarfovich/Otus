#include "duplicate_finder/finder_task.h"
#include "duplicate_finder/duplicate_finder.h"
#include "duplicate_finder/string_vector.h"
#include "duplicate_finder/digester_factory.h"

#include <boost/program_options.hpp>

#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>

namespace po = boost::program_options;

using FinderPreparationResult = std::pair<std::optional<FinderTask>, StringVector>;

std::atomic_bool taskFinished { false };

void onTaskFinished()
{
    taskFinished.store(true);
}

void                    fillOptionsDescription(po::options_description& options);
FinderPreparationResult prepareFinderTask(const po::variables_map& options);
void                    printErrors(const StringVector& errors);
void                    printSearchingTaskMessage(const FinderTask& task);
void                    printResults(const Duplicates& duplicates);
void                    printErrorsOrWarnings(const FinderPreparationResult& result);

int main(int argc, char* argv[])
{
    try {
        po::options_description programOptions { "Command-line options" };
        fillOptionsDescription(programOptions);

        po::variables_map optionVariables;
        po::store(po::parse_command_line(argc, argv, programOptions), optionVariables);
        po::notify(optionVariables);

        if (optionVariables.count("help")) {
            std::cout << programOptions << "\n";
            return 0;
        }

        FinderPreparationResult result;
        try {
            result = prepareFinderTask(optionVariables);
        } catch (const std::exception& e) {
            std::cout << std::string { "Unhandled error: " } + e.what() << '\n';
        }

        printErrorsOrWarnings(result);
        if (!result.first) {
            return 0;
        }

        const auto& finderTask = result.first.value();
        printSearchingTaskMessage(finderTask);

        DuplicateFinder finder;
        finder.taskFinished.connect(onTaskFinished);
        finder.findDuplicates(finderTask);

        while (!taskFinished) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        printResults(finder.duplicates());

    } catch (const std::exception& e) {
        std::cout << std::string { "Unhandled error: " } + e.what() + ".\nPlease report this error to Elon Musk.";
    }

    return 0;
}

void fillOptionsDescription(po::options_description& options)
{
    options.add_options()("help", "Show current message.");
    options.add_options()("algorithm", po::value<std::string>(), "Hash algorithm (case-insensitive). Supported algorithms: dummy, SHA3");
    options.add_options()("targets",
                          po::value<std::vector<std::string>>()->multitoken(),
                          "List of target folders to search for duplicate files. Delimit folders with spaces.");
    options.add_options()("blackList",
                          po::value<std::vector<std::string>>()->multitoken(),
                          "List of folders to exclude from search. Delimit folders with spaces.");
    options.add_options()("nonRecursive", po::bool_switch()->default_value(true), "Disable recursive search.");
    options.add_options()("size", po::value<std::size_t>(), "Minimal size of file to perform comparison.");
    options.add_options()("masks",
                          po::value<std::vector<std::string>>()->multitoken(),
                          "List of file extensions. Delimit extensions with spaces.");
    options.add_options()("blockSize", po::value<std::size_t>(), "Size of block to read files.");
    options.add_options()("threads",
                          po::value<unsigned>(),
                          "Threads count to use. 0 - auto determine number of threads (default value).");
}

std::pair<std::optional<FinderTask>, StringVector> prepareFinderTask(const po::variables_map& options)
{
    FinderTask   task;
    StringVector errors;

    task.digester = std::make_shared<DummyDigester>();


    DigesterFactory digesterFactory;
    if (options.count("algorithm") == 0){
        task.digester = digesterFactory.createDigester("dummy");
    } else{
        task.digester = digesterFactory.createDigester(options["algorithm"].as<std::string>());
        if (!task.digester){
            errors.push_back("Failed to create algorithm with name " + options["algorithm"].as<std::string>());
            return { {}, errors };
        }
    }

    if (options.count("targets") == 0) {
        errors.push_back("No target folder. Please, specify at least one.");
        return { {}, errors };
    }

    const auto& targets = options["targets"].as<StringVector>();
    for (const auto& target : targets) {
        auto path     = std::filesystem::path { target };
        bool hasError = false;
        if (!std::filesystem::is_directory(path)) {
            errors.push_back(target + " - is not a folder. Skipping it.");
            hasError = true;
        }
        if (!std::filesystem::exists(path)) {
            errors.push_back(target + " - doesn't exist. Skipping it.");
            hasError = true;
        }
        if (!hasError) {
            try {
                task.targets.push_back(std::filesystem::canonical(std::move(path)));
            } catch (const std::exception& e) {
                errors.push_back(target + " - failed to resolve the path (error: " + e.what() + "). Skipping it.");
            }
        }
    }
    if (task.targets.empty()) {
        errors.push_back("No suitable folders to search.");
        return { {}, errors };
    }

    if (options.count("blackList") > 0) {
        const auto& blackListed = options["blackList"].as<StringVector>();
        for (const auto& entry : blackListed) {
            task.blackList.push_back(entry);
        }
    }

    if (options.count("recursive") > 0) {
        task.nonRecursiveSearch = true;
    }

    if (options.count("size") > 0) {
        task.minimalFileSize = options["size"].as<std::size_t>();
    }

    if (options.count("masks") > 0) {
        task.extensionsMasks = options["masks"].as<StringVector>();
    }

    if (options.count("blockSize") > 0) {
        task.blockSize = options["blockSize"].as<std::size_t>();
    }

    if (options.count("threads") > 0) {
        task.maxThreadCount = options["threads"].as<unsigned>();
    }

    return { std::move(task), std::move(errors) };
}

void printErrors(const StringVector& errors)
{
    for (const auto& error : errors) {
        std::cout << "  " << error << '\n';
    }
}

void printSearchingTaskMessage(const FinderTask& task)
{
    std::cout << "Performing search for duplicate files with the following options:\n";
    std::cout << "  Algorithm: " << task.digester->name() << '\n';
    std::cout << "  Target folders:\n";
    for (const auto& target : task.targets) {
        std::cout << "    " << target << '\n';
    }
    std::cout << "  Excluding folders:\n";
    for (const auto& black : task.blackList) {
        std::cout << "    " << black << '\n';
    }
    std::cout << "  Recursive search: " << std::boolalpha << task.nonRecursiveSearch << '\n';
    std::cout << "  Minimal file size: " << task.minimalFileSize << '\n';
    std::cout << "  Search for files with extensions:\n";
    if (task.extensionsMasks.empty()) {
        std::cout << "    (ALL)\n";
    } else {
        for (const auto& extension : task.extensionsMasks) {
            std::cout << "    " << extension << '\n';
        }
    }
    std::cout << "  Block size: " << task.blockSize << '\n';
    std::cout << "  Threads count: " << task.maxThreadCount << '\n';
    std::cout << "\nSearching...\n\n";
}

void printResults(const Duplicates& duplicates)
{
    const auto& resultsMap = duplicates.duplicatesMap();
    std::cout << "Searching finished.\n";
    if (resultsMap.empty()) {
        std::cout << "No duplicates found!\n";
    } else {
        std::cout << "Duplicates found:\n";
        std::size_t counter = 1;
        for (const auto& [hash, files] : resultsMap) {
            std::cout << "Group " << counter << ":\n";
            for (const auto& file : files) {
                std::cout << "  " << file->path() << '\n';
            }
            ++counter;
        }
    }
}

void printErrorsOrWarnings(const FinderPreparationResult& result)
{
    if (!result.first) {
        std::cout << "Failed to create search task. Errors are:\n";
        printErrors(result.second);
        std::cout << "Please refer to help message (call with '--help' argument)\n";
        std::cout << "Going for a cup of coffee...\n";
    } else if (!result.second.empty()) {
        std::cout << "Warnings:\n";
        printErrors(result.second);
        std::cout << "Failed requested will be omitted\n";
    }
}
