#pragma once

#include "path_vector.h"
#include "file_properties.h"
#include "file_properties_hasher.h"
#include "finder_task.h"
#include "path_utils.h"

#include <unordered_set>
#include <iostream>

class FileFinder
{
public:
    std::unordered_set<FileProperties, FilePropertiesHasher> findFiles(const FinderTask& task) const
    {
        std::unordered_set<FileProperties, FilePropertiesHasher> files;
        for (const auto& target : task.targets) {
            if (pathIsUnder(target, task.blackList)) {
                continue;
            }

            if (std::filesystem::is_regular_file(target) && fileExtensionMatches(target, task.extensionsMasks)) {
                files.insert({ target, std::filesystem::file_size(target) });
            } else {
                auto iter = std::filesystem::recursive_directory_iterator { target };
                if (!task.recursiveSearch) {
                    iter.disable_recursion_pending();
                }
                for (const auto& entry : iter) {
                    if (std::filesystem::is_regular_file(entry) && fileExtensionMatches(entry, task.extensionsMasks)) {
                        auto result = files.insert({ entry, std::filesystem::file_size(entry) });
                        if(!result.second){
                            std::cout << "SKIPPED "
                                      << files.contains({ entry, std::filesystem::file_size(entry) }) << " "
                                      << entry << '\n';
                        }
                    }
                    if (!task.recursiveSearch) {
                        iter.disable_recursion_pending();
                    }
                }
            }
        }

        return files;
    }
};