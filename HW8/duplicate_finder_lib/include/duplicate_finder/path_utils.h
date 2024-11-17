#pragma once

#include "string_vector.h"
#include "path_vector.h"

#include <filesystem>

bool fileExtensionMatches(const std::filesystem::path& path, const StringVector& extensionMasks)
{
    if(extensionMasks.empty()){
        return true;
    }

    if(!std::filesystem::is_regular_file(path)){
        return true;
    }

    //std::filesystem::path path     = "/path/to/file.tar.gz";
    //std::string           extension(std::find(filename.begin(), filename.end(), '.'), filename.end());

    const auto filename = path.filename().string();
    const std::string extension = {std::find(filename.begin(), filename.end(), '.'), filename.end()};
    for (const auto& extensionMask : extensionMasks){
        if (extension == extensionMask){
            return true;
        }
    }

    return false;
}

bool pathIsUnder(std::filesystem::path path, const PathVector& blackList)
{
    if (path.filename().empty()){
        path = path.parent_path();
    }
    for (auto blackEntry : blackList) {
        if(blackEntry.filename().empty()){
            blackEntry = blackEntry.parent_path();
        }
        const auto mergedPath     = blackEntry / path;
        const auto [iter1, iter2] = std::mismatch(blackEntry.begin(), blackEntry.end(), mergedPath.begin());

        if (iter1 == blackEntry.end()) {
            return true;
        }
    }
    return false;
}