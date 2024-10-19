#pragma once

#include "file_properties.h"

#include <cstddef>

#include <iostream>

struct FilePropertiesHasher
{
    std::size_t operator()(const FileProperties& p) const { 
        std::cout << "Hash: "
                  << std::hash<std::filesystem::path>()(p.path()) << " " << p.path() << '\n';
        return std::hash<std::filesystem::path>()(p.path());
    }
};