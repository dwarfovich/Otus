#pragma once

#include "database_error.h"

#include <map>
#include <optional>
#include <vector>
#include <string>

class DatabaseResult
{
    friend class Database;

private: // types
    using Data           = std::map<int, std::vector<std::string>>;

public: // types
    using iterator = Data::iterator;
    using const_iterator = Data::const_iterator;

    public: // methods
    DatabaseResult() = default;
        DatabaseResult(DatabaseError error): error_{error}{}

    std::optional<DatabaseError> error() const { return error_; }
    int errorData() const { return errorData_;}

    const_iterator begin() const { return data_.begin(); }
    const_iterator end() const { return data_.end(); }
    const_iterator cbegin() const { return data_.cbegin(); }
    const_iterator cend() const { return data_.cend(); }

private: // data
    std::optional<DatabaseError> error_;
    int errorData_ = 0;
    Data                         data_;
};