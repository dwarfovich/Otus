#include "duplicate_finder/digester.h"
#include "duplicate_finder/finder_task.h"
#include "duplicate_finder/string_vector_hasher.h"
#include "duplicate_finder/duplicate_finder.h"
#include "duplicate_finder/file_finder.h"

#include <boost/program_options.hpp>
#include <boost/version.hpp>

#include <cryptopp/cryptlib.h>
#include <cryptopp/sha3.h>
#include <cryptopp/hex.h>

#include <iostream>

int main(int argc, char* argv[])
{
    FinderTask t;

    using Group = std::vector<std::filesystem::path>;
    std::unordered_map<StringVector, Group, StringVectorHasher> map;

    DuplicateFinder df;
    FileFinder      ff;
    FinderTask      task;
    task.recursiveSearch = true;
    task.targets.push_back("C:\\Boo\\Temp");
    //auto files = ff.findFiles(task);
    //for (const auto& fileProperty : files){
        //std::cout << fileProperty.path() << " " << fileProperty.size() << '\n';
    //}


        std::unordered_set<FileProperties, FilePropertiesHasher> ffiles;
    auto iter = std::filesystem::recursive_directory_iterator { "C:\\Boo\\Temp" };
    for (const auto& entry : iter) {
        if (std::filesystem::is_regular_file(entry) && fileExtensionMatches(entry, task.extensionsMasks)) {
            auto result = ffiles.insert({ entry, std::filesystem::file_size(entry) });
            if(!result.second){
                //std::cout << "SKIPPED " << std::boolalpha << ffiles.contains({entry, 0}) << " " << entry << '\n';
            }
    //        std::cout << entry << " " << '\n';
        }
    }

    for(const auto& f : ffiles){
        std::cout << f.path() << '\n';
    }

    return 0;
}
