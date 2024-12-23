#pragma once

#include "path_vector.h"
#include "string_vector.h"
#include "digester.h"

#include <string>
#include <memory>

struct FinderTask
{
    std::shared_ptr<Digester> digester = nullptr;

    PathVector   targets;
    PathVector   blackList;
    bool         nonRecursiveSearch = false;
    std::size_t  minimalFileSize = 0;
    StringVector extensionsMasks;
    std::size_t  blockSize                   = 1024;
    unsigned     maxThreadCount              = 0;
};