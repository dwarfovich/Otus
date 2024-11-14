#pragma once

#include "digest_blocks.h"
#include <boost/container_hash/hash.hpp>

struct DigestBlocksHasher
{
    std::size_t operator()(const DigestBlocks& blocks) const { 
        std::size_t hash = 0;
        for (const auto& block : blocks){
            boost::hash_combine(hash, block);
        }

        return hash;
    }
};