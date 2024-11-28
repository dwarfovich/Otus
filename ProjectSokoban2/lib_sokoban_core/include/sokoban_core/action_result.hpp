#pragma once

#include <any>
#include <vector>

class ActionResult{
public:

    private:
        std::string id_;
        std::vector<std::any> data_;
};