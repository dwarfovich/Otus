#pragma once

#include "file_digest_map.h"

struct FilePropertiesIterComparator
{
    bool operator()(const std::vector<FileProperties>::const_iterator& lhs,
                    const std::vector<FileProperties>::const_iterator& rhs) const
    {
        return lhs->hash() == rhs->hash();
    }
};