#include "duplicate_finder/file_properties.h"
#include "duplicate_finder/duplicate_finder.h"

#include <gtest/gtest.h>

#include <vector>

std::vector<FileProperties> generateFileProperties(std::size_t count)
{
    std::vector<FileProperties> files;
    for (std::size_t i = 0; i < count; ++i) {
        files.push_back({ "file_" + std::to_string(i), 1 });
    }

    return files;
}
