#pragma once

#include "path_vector.h"
#include "file_properties.h"
#include "finder_task.h"
#include "path_utils.h"
#include "source_vector.h"

#include <unordered_map>
#include <iostream>

class FileFinder
{
public: // types
    using FileMap = std::unordered_map<std::uintmax_t, SourceVector>;

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
                    files[std::filesystem::file_size(target)].push_back(std::make_unique<FileSource>(target));
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
                map[std::filesystem::file_size(entry)].push_back(std::make_unique<FileSource>(entry));
            }
        }
    }
};