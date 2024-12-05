#pragma once

#include <string>
#include <memory>

namespace sokoban {

class GameObjectId
{
    friend auto operator==(const sokoban::GameObjectId& lhs, sokoban::GameObjectId rhs)
    {
        return lhs.objectId_ == rhs.objectId_;
    }
    friend auto operator==(const sokoban::GameObjectId& lhs, const char* rhs) { return lhs.objectId_ == rhs; }

public:
    GameObjectId(const std::string& id) : objectId_ { id } {}

    const std::string& id() const { return objectId_; }

private:
    std::string objectId_;
};

using GameObjectIdSptr = std::shared_ptr<GameObjectId>;

} // namespace sokoban
