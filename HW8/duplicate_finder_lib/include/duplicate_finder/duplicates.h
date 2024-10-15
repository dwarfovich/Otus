#pragma once

#include "string_vector.h"
#include "string_vector_hasher.h"

#include <filesystem>
#include <unordered_map>

class Duplicates
{
public:
    using Group = std::vector<std::filesystem::path>;

private:
    std::unordered_map<StringVector, Group, StringVectorHasher> duplicates_;
};