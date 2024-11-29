#pragma once

#include <string>
#include <memory>

namespace sokoban {

class GameObjectId
{
public:
private:
    std::string objectId_;
    std::string sourceId_;
};

using GameObjectIdSptr = std::shared_ptr<GameObjectId>;

} // namespace sc