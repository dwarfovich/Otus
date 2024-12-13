#pragma once

#include "json_utils/json_utils.hpp"

#include <string>
#include <unordered_set>
#include <filesystem>
#include <iostream>

namespace sokoban {

class Player
{
public:
    Player() = default;
    Player(const nlohmann::json::value_type& playerObject) {}

    const std::string& name() const{ return name_; }
    void               addAchievement(const std::filesystem::path& modFolder, const std::string& message)
    {
        achievements_[modFolder].insert(message);
    }
private:
    std::string name_;
    using AchievementsSet = std::unordered_set<std::string>;
    std::unordered_map<std::filesystem::path, AchievementsSet> achievements_;
};

std::vector<Player> loadPlayersList(const json_utils::Json& json)
{
    std::vector<Player> players;
    for (const auto& playerObject : json) {
        try {
            players.push_back(Player { playerObject });
        } catch (const std::exception& e) {
            std::cout << "Exception: " << e.what() << '\n';
        }
    }

    return players;
}

} // namespace sokoban