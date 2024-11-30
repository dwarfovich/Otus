#pragma once

//#include "nlohmann/json.hpp"
#include "nlohmann/json.hpp"


#include <filesystem>
#include <fstream>

namespace sokoban{
namespace json_utils{
    using Json = nlohmann::json;

    inline Json loadFromFile(const std::filesystem::path& path){
        std::ifstream stream {path};
        if(!stream.is_open()){
            throw std::runtime_error("Coudn't open file " + path.string());
        }
       return Json::parse(stream);
    }
}
}