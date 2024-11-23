#pragma once

#include "database_error.h"
#include "database_result.h"

#include <gtest/gtest_prod.h>

#include <string>
#include <unordered_map>
#include <optional>
#include <mutex>

class Database
{
    FRIEND_TEST(Database, TestInsert);
    FRIEND_TEST(Database, TestInsertSuccess);
    FRIEND_TEST(Database, TestTruncate);
    

private: // types
    using Table = std::unordered_map<int, std::string>;

public:
    DatabaseResult insert(std::size_t tableId, int id, std::string value)
    {
        std::lock_guard lock { mutex_ };
        auto*           table = findTable(tableId);
        if (!table) {
            return DatabaseError::TableNotFound;
        }

        const auto [iter, success] = table->insert({ id, std::move(value) });
        if (!success) {
            DatabaseResult result { DatabaseError::ValueAlreadyExists };
            result.errorData_ = id;
            return result;
        }

        return {};
    }

    DatabaseResult clear(std::size_t tableId)
    {
        std::lock_guard lock { mutex_ };
        Table*          table = findTable(tableId);
        if (!table) {
            return DatabaseError::TableNotFound;
        }

        table->clear();

        return {};
    }

    DatabaseResult intersection() const
    {
        std::lock_guard lock { mutex_ };
        DatabaseResult  result;
        for (const auto& [id, value] : table1_) {
            const auto iter = table2_.find(id);
            if (iter != table2_.cend()) {
                result.data_.insert({ id, { value, iter->second } });
            }
        }

        return result;
    }

    DatabaseResult symmetricDifference() const
    {
        std::lock_guard lock { mutex_ };
        DatabaseResult  result;
        for (const auto& [id, value] : table1_) {
            if (table2_.find(id) == table2_.cend()) {
                result.data_.insert({ id, { value, "" } });
            }
        }
        for (const auto& [id, value] : table2_) {
            if (table1_.find(id) == table1_.cend()) {
                result.data_.insert({ id, { "", value } });
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

    const Table* findTable(std::size_t id) const { return findTable(id); }

private: // data
    std::unordered_map<int, std::string> table1_;
    std::unordered_map<int, std::string> table2_;
    mutable std::mutex                   mutex_;
};