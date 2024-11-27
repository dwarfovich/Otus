#pragma once

//#include "nlohmann/json.hpp"
#include "nlohmann/json.hpp"


#include <filesystem>
#include <fstream>

namespace sokoban{
namespace json_utils{
    using Json = nlohmann::json;

    Json loadFromFile(const std::filesystem::path& path){
        std::ifstream stream {path};

       return Json::parse(stream);
    }
}
}