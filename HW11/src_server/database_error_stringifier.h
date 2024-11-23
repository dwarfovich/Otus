#pragma once

#include "db/database_error.h"

#include <string>

inline std::string toString(DatabaseError error){
    switch(error){
        case DatabaseError::TableNotFound: return "table_not_found";
        case DatabaseError::ValueAlreadyExists: return "duplicate";
        default: return "unknown_error";
    }
}