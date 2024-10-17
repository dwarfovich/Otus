#pragma once

#include "file_properties.h"

#include <cstddef>

struct FilePropertiesHasher
{
    std::size_t operator()(const FileProperties& p) const { 
        return std::hash<std::filesystem::path>()(p.path());
    }
};