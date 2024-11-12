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
    using FileMap = std::unordered_map<std::uintmax_t, PathVector>;

public: // methods
    FileMap findFiles(const FinderTask& task) const
    {
        FileMap files;
        for (const auto& target : task.targets) {
            if (pathIsUnder(target, task.blackList)) {
                continue;
            }

            if (std::filesystem::is_regular_file(target)) {
                if (fileExtensionMatches(target, task.extensionsMasks)) {
                    files[std::filesystem::file_size(target)].push_back(target);
                }
            } else if (std::filesystem::is_directory(target) && task.recursiveSearch) {
                addFilesFromFolder(target, task, files);
            }
        }

        return files;
    }

private:
    void addFilesFromFolder(const std::filesystem::path& path, const FinderTask& task, FileMap& map) const
    {
        auto iter = std::filesystem::recursive_directory_iterator { path };
        for (const auto& entry : iter) {
            if (std::filesystem::is_regular_file(entry) && fileExtensionMatches(entry, task.extensionsMasks)) {
                map[std::filesystem::file_size(entry)].push_back(entry);
            }
        }
    }
};