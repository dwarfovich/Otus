#pragma once

#include "digest_blocks.h"
#include "digest_blocks_hasher.h"
#include "source_entity.h"

#include <filesystem>
#include <unordered_map>
#include <unordered_set>
#include <shared_mutex>

class Duplicates
{
public:
    using Group = std::unordered_set<std::shared_ptr<SourceEntity>>;
    using DuplicatesUmap = std::unordered_map<DigestBlocks, Group, DigestBlocksHasher>;

    void addDuplicate(const DigestBlocks& digest, const std::shared_ptr<SourceEntity>& source)
    {
        std::lock_guard lock {mutex_};
        duplicates_[digest].insert(source);
    }

    const DuplicatesUmap& duplicates() const noexcept{
        return duplicates_;
    }

private:
    DuplicatesUmap duplicates_;
    std::shared_mutex mutex_;
};