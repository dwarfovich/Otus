#pragma once

#include "database_error.h"
#include "algebraic_result.h"

#include <string>
#include <unordered_map>
#include <optional>

class Database
{
private: // types
    using Table = std::unordered_map<int, std::string>;

public:
    std::optional<DatabaseError> insert(std::size_t tableId, int id, std::string value)
    {
        Table* table = findTable(tableId);
        if (!table) {
            return DatabaseError::TableNotFound;
        }

        const auto [iter, success] = table->insert({ id, std::move(value) });
        if (!success) {
            return DatabaseError::ValueAlreadyExists;
        }

        return {};
    }

    std::optional<DatabaseError> clear(std::size_t tableId)
    {
        Table* table = findTable(tableId);
        if (!table) {
            return DatabaseError::TableNotFound;
        }

        table->clear();

        return {};
    }

    AlgebraicResult intersection() const{
        AlgebraicResult result;
        result.type_ = AlgebraicResultType::Intersection;
        for (const auto& [id, value] : table1_){
            const auto iter = table2_.find(id);
            if(iter != table2_.cend()){
                result.data_.insert({id, {value, iter->second}});
            }
        }

        return result;
    }

private: // methods
    Table* findTable(std::size_t id)
    {
        if (id == 1) {
            return &table1_;
        } else if (id == 2) {
            return &table2_;
        } else {
            return nullptr;
        }
    }

private: // data
    std::unordered_map<int, std::string> table1_;
    std::unordered_map<int, std::string> table2_;
};