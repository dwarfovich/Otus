#pragma once

#include "file_properties.h"

#include <boost/container_hash/hash.hpp>

#include <cstddef>

struct FilePropertiesHasher
{
    std::size_t operator()(const FileProperties& p) const
    {
        return std::hash<std::filesystem::path>()(p.path());
    }
};

struct FilePropertiesIterHasher
{
    using is_transparent = void;

    std::size_t operator()(const std::vector<FileProperties>::const_iterator& iter) const
    {
        return hashForDigest(iter->hash());
    }
    std::size_t operator()(const std::vector<std::string>& digest) const { return hashForDigest(digest); }

private:
    std::size_t hashForDigest(const std::vector<std::string>& digest) const
    {
        std::size_t hash = 0;
        for (const auto& hashPart : digest) {
            boost::hash_combine(hash, hashPart);
        }

        return hash;
    }
};