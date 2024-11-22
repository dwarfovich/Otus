#pragma once

#include <cinttypes>

enum class DatabaseError : uint8_t{
    TableNotFound,
    ValueAlreadyExists
};