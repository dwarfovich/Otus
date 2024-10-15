#pragma once

#include "paths_vector.h"
#include "strings_vector.h"
#include "digester.h"

#include <string>
#include <memory>

struct FinderTask
{
    std::shared_ptr<Digester> digester = nullptr;
    PathsVector targets;
    PathsVector blackList;
    bool recursiveSearch = false;
    std::size_t minimalFileSize = 1;
    StringsVector extensionsMasks;
    std::size_t blockSize = 1024;
};