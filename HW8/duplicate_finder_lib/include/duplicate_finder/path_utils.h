#pragma once

#include "string_vector.h"
#include "path_vector.h"

#include <filesystem>

bool fileExtensionMatches(const std::filesystem::path& path, const StringVector& extensionMasks)
{
    if (extensionMasks.empty()) {
        return true;
    }

    if (!std::filesystem::is_regular_file(path)) {
        return true;
    }

    const auto&       filename  = path.filename().string();
    size_t            pos       = filename.find('.');
    const std::string extension = { (pos == std::string::npos ? filename.cend() : filename.cbegin() + pos),
                                    filename.cend() };
    for (const auto& extensionMask : extensionMasks) {
        if (extension == extensionMask) {
            return true;
        }
    }

    return false;
}

bool pathIsUnder(std::filesystem::path path, const PathVector& blackList)
{
    if (path.filename().empty()) {
        path = path.parent_path();
    }
    if(blackList.size() == 1){
        int t = 43;
    }
    for (auto blackEntry : blackList) {
        if (blackEntry.filename().empty()) {
            blackEntry = blackEntry.parent_path();
        }
        const auto [iter1, iter2] = std::mismatch(blackEntry.begin(), blackEntry.end(), path.begin(), path.end());

        if (iter1 == blackEntry.end()) {
            return true;
        }
    }
    return false;
}