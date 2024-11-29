#pragma once

#include "game_object_factory.hpp"

namespace sokoban{
    namespace sbg{
    
        class BaseGameObjectFactory : public GameObjectFactory{
        public:
            GameObjectUptr create(const GameObjectId& id) const override {
                return nullptr;
            }
        };

    }
}