#pragma once

#include "algebraic_result_type.h"

#include <string>
#include <vector>
#include <map>

class AlgebraicResult
{
    friend class Database;

private: // types
    using Data = std::map<int, std::vector<std::string>>;

public:
    using const_iterator = Data::const_iterator;

    AlgebraicResultType type() const { return type_;} 
    const_iterator cbegin() const { return data_.cbegin(); }
    const_iterator cend() const { return data_.cend(); }

private: // data
    AlgebraicResultType type_ = AlgebraicResultType::Unknown; 
    Data data_;
};