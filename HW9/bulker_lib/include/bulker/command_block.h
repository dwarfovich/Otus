#pragma once

class CommandIdentifier;

#include <ctime>
#include <vector>

class CommandBlock
{
public:
    const std::time_t& timestamp() const { return timestamp_;}
    void setTimestamp(const std::time_t& timestamp) { timestamp_ = timestamp; }
    void setTimestampIfAbcent(const std::time_t& timestamp) {
        if (timestamp_ == 0) {
            timestamp_ = timestamp;
        }}
    void addCommandIdentifier(CommandIdentifier&& id) {
        identifiers_.push_back(std::move(id));
    }
    void clear() {identifiers_.clear();}
    const std::vector<CommandIdentifier>& identifiers() const { return identifiers_; }

private:
    std::time_t                    timestamp_ = 0;
    std::vector<CommandIdentifier> identifiers_;
};
