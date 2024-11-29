#pragma once

#include <any>
#include <vector>

namespace sokoban{

    class ActionResult{
public:

    private:
        std::string id_;
        std::vector<std::any> data_;
};

}