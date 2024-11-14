#pragma once

#include "digest_blocks.h"
#include "digest_blocks_hasher.h"

#include <filesystem>
#include <unordered_map>
#include <unordered_set>
#include <shared_mutex>

class Duplicates
{
public:
    using Group = std::unordered_set<std::filesystem::path>;
    using DuplicatesUmap = std::unordered_map<DigestBlocks, Group, DigestBlocksHasher>;

    void addDuplicate(const DigestBlocks& digest, const std::filesystem::path& path)
    {
        std::lock_guard lock {mutex_};
        duplicates_[digest].insert(path);
    }

    const DuplicatesUmap& duplicates() const noexcept{
        return duplicates_;
    }

private:
    DuplicatesUmap duplicates_;
    std::shared_mutex mutex_;
};