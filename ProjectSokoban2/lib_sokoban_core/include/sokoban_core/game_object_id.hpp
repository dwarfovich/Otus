#pragma once

#include <string>
#include <memory>

namespace sokoban {

class GameObjectId
{
public:
    GameObjectId(const std::string& id) : objectId_{id}{}

    const std::string& id() const {return objectId_;}
private:
    std::string objectId_;
    std::string sourceId_;
};

using GameObjectIdSptr = std::shared_ptr<GameObjectId>;

} // namespace sc