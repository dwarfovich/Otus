#pragma once

class CommandIdentifier;

#include <ctime>
#include <vector>

class CommandBlock
{
public:
    void setTimestamp(const std::time_t& timestamp) { timestamp_ = timestamp; }
    void addCommandIdentifier(CommandIdentifier&& id) { identifiers_.push_back(std::move(id)); }

    const std::vector<CommandIdentifier>& identifiers() const { return identifiers_; }

private:
    std::time_t                    timestamp_;
    std::vector<CommandIdentifier> identifiers_;
};
