#pragma once

#include "player.hpp"
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
    Player(const nlohmann::json::value_type& playerObject) {
        auto iter = playerObject.find("name");
        if (iter == playerObject.cend()){
            throw std::runtime_error("Failed to load player name");
        }
        name_ = iter->get<std::string>();

        iter = playerObject.find("achievements");
        if (iter == playerObject.cend()) {
            return;
        }

        try{
        for (const auto& [path, message] : playerObject.items()){
            achievements_[path] = message;
        }
        } catch(const std::exception& e){
            std::string error = e.what();
            int t = 63;
        }
        int t = 5243;
    }

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

inline std::vector<std::shared_ptr<Player>> loadPlayersList(const json_utils::Json& json)
{
    std::vector<std::shared_ptr<Player>> players;
    for (const auto& playerObject : json) {
        try {
            players.push_back(std::make_shared<Player> ( playerObject ));
        } catch (const std::exception& e) {
            std::cout << "Exception: " << e.what() << '\n';
        }
    }

    return players;
}

} // namespace sokoban