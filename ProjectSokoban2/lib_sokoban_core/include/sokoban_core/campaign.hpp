#pragma once

#include "json_utils/json_utils.hpp"

#include <vector>
#include <istream>
#include <string>

namespace sokoban {

class Campaign
{
public:
    void load(const json_utils::Json json)
    {
        auto iter = json.find("levels");
        if(iter.value().is_array()){
            for (const auto& level : iter.value()){
                levelNames_.push_back(level.get<std::string>());
            }
        }
    }

    void setCurrentLevel(std::size_t level)
    {
        if (level < levelNames_.size()) {
            currentLevel_ = level;
        }
    }

    std::size_t currentLevel() const{
        return currentLevel_;
    }

    const std::string& currentLevelName() const{
        return levelNames_.at(currentLevel_);
    }

    bool hasNextLevel() const{
        levelNames_.size() > currentLevel_ + 1;
    }

private:
    std::size_t              currentLevel_ = 0;
    std::vector<std::string> levelNames_;
};

} // namespace sokoban