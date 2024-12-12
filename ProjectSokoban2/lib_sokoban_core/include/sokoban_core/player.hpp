#pragma once

#include <string>
#include <unordered_map>
#include <filesystem>

namespace sokoban {

class Player
{
public:
private:
    std::string name_;
    using AchievementsMap = std::unordered_map<std::string, std::size_t>;
    std::unordered_map<std::filesystem::path, AchievementsMap> modAchievements_;
};

} // namespace sokoban