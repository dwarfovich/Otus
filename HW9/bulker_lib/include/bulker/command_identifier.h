#pragma once

#include <string>

class CommandIdentifier
{
public:
    CommandIdentifier(const std::string& id) : id_{id}{}

    const std::string& identifier() const noexcept {
        return id_;
    }

    private:
        std::string id_;
};