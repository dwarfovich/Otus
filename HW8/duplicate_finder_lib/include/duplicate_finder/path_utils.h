#pragma once

#include "string_vector.h"
#include "path_vector.h"

#include <filesystem>

bool fileExtensionMatches(const std::filesystem::path& path, const StringVector& extensions){
    return true;
}

bool pathIsUnder(const std::filesystem::path& path, const PathVector& blackList){
    return false;
}