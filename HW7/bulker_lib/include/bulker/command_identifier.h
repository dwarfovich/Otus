#pragma once

#include <string>

class CommandIdentifier
{
public:
    const std::string& identifier() const noexcept {
        return id_;
    }

    private:
        std::string id_;
};