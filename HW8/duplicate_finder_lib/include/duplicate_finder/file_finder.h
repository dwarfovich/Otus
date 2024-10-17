#pragma once

#include "path_vector.h"
#include "file_properties.h"
#include "file_properties_hasher.h"
#include "finder_task.h"
#include "path_utils.h"

#include <unordered_set>

class FileFinder{
public:
    std::unordered_set<FileProperties, FilePropertiesHasher> findFiles(const FinderTask& task) const {
        std::unordered_set<FileProperties, FilePropertiesHasher> files;
        for (const auto& target : task.targets){
            if(pathIsUnder(target, task.blackList)){
                continue;
            }
            if(std::filesystem::is_regular_file(target)){
                files.insert({ target, std::filesystem::file_size(target) });
            }
        }
    }
};