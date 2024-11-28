#pragma once

#include <filesystem>

namespace sokoban{

    class NewGameParameters{
    public:
        std::filesystem::path modFolder;
    };
}