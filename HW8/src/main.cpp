#include "duplicate_finder/finder_task.h"
#include "duplicate_finder/duplicate_finder.h"
#include "duplicate_finder/string_vector.h"

#include <boost/program_options.hpp>
#include <boost/version.hpp>

#include <cryptopp/cryptlib.h>
#include <cryptopp/sha3.h>
#include <cryptopp/hex.h>

#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>

std::atomic_bool taskFinished { false };

void onTaskFinished()
{
    std::cout << "task finished (thread id=" << std::this_thread::get_id() << ")\n";
    taskFinished.store(true);
}

namespace po = boost::program_options;

po::options_description optionsDescription { "Command-line options" };

void                                               fillOptionsDescription();
std::pair<std::optional<FinderTask>, StringVector> prepareFinderTask(const po::variables_map& options);
void                                               printErrors(const StringVector& errors);

int main(int argc, char* argv[])
{
    fillOptionsDescription();

    po::variables_map options;
    po::store(po::parse_command_line(argc, argv, optionsDescription), options);
    po::notify(options);

    if (options.count("help")) {
        std::cout << optionsDescription << "\n";
        return 0;
    }

    const auto& result = prepareFinderTask(options);
    if (!result.first) {
        std::cout << "Failed to create search task. Errors are:\n";
        printErrors(result.second);
        std::cout << "Please refer to help message (call with '--help' argument)\n";
        std::cout << "Going for a cup of coffee...\n";
        return 0;
    } else if (!result.second.empty()) {
        std::cout << "Warnings:\n";
        printErrors(result.second);
        std::cout << "Failed requested will be omitted\n";
    }

    return 0;
}

/*
PathVector   targets;
    PathVector   blackList;
    bool         recursiveSearch = false;
    std::size_t  minimalFileSize = 0;
    StringVector extensionsMasks;
    std::size_t  blockSize                   = 1024;
    unsigned     maxThreadCount              = 0;
    */
void fillOptionsDescription()
{
    optionsDescription.add_options()("help", "Show current message.");
    optionsDescription.add_options()(
        "targets",
        po::value<std::vector<std::string>>()->multitoken(),
        "List of target folders to search for duplicate files. Delimit folders with spaces.");
    optionsDescription.add_options()("blackList",
                                     po::value<std::vector<std::string>>()->multitoken(),
                                     "List of folders to exclude from search. Delimit folders with spaces.");
    optionsDescription.add_options()("recursive", po::value<bool>(), "Enable recursive search.");
    optionsDescription.add_options()("size", po::value<std::size_t>(), "Minimal size of file to perform comparison.");
    optionsDescription.add_options()("masks",
                                     po::value<std::vector<std::string>>()->multitoken(),
                                     "List of file extensions. Delimit extensions with spaces.");
    optionsDescription.add_options()("blockSize", po::value<std::size_t>(), "Size of block to read files.");
    optionsDescription.add_options()("threads",
                                     po::value<unsigned>(),
                                     "Threads count to use. 0 - auto determine number of threads (default value).");
}

std::pair<std::optional<FinderTask>, StringVector> prepareFinderTask(const po::variables_map& options)
{
    FinderTask   task;
    StringVector errors;
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
            task.targets.push_back(std::move(path));
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
        task.recursiveSearch = true;
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

    return {std::move(task), std::move(errors)};
}

void printErrors(const StringVector& errors)
{
    for (const auto& error : errors) {
        std::cout << "  " << error << '\n';
    }
}