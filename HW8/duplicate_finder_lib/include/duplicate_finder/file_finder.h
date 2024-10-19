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
public: // types
    using FilePropertiesUSet = std::unordered_set<FileProperties, FilePropertiesHasher>;

public: // methods
    FilePropertiesUSet findFiles(const FinderTask& task) const
    {
        std::unordered_set<FileProperties, FilePropertiesHasher> files;
        for (const auto& target : task.targets) {
            if (pathIsUnder(target, task.blackList)) {
                continue;
            }

            if (std::filesystem::is_regular_file(target)) {
                if (fileExtensionMatches(target, task.extensionsMasks)) {
                    files.insert({ target, std::filesystem::file_size(target) });
                }
            } else if (std::filesystem::is_directory(target) && task.recursiveSearch) {
                addFilesFromFolder(target, task, files);
            }
        }

        return files;
    }

private:
    void addFilesFromFolder(const std::filesystem::path& path, const FinderTask& task, FilePropertiesUSet& set) const
    {
        auto iter = std::filesystem::recursive_directory_iterator { path };
        for (const auto& entry : iter) {
            if (std::filesystem::is_regular_file(entry) && fileExtensionMatches(entry, task.extensionsMasks)) {
                auto result = set.insert({ entry, std::filesystem::file_size(entry) });
            }
        }
    }
};