#pragma once

#include "request.h"
#include "request_factory.h"
#include "database_error_stringifier.h"

#include "db/database.h"

#include <string>
#include <memory>
#include <optional>

class RequestProcessor
{
public:
    RequestProcessor(const std::shared_ptr<Database>& database) : db_ { database } {}

    std::optional<std::string> process(const std::string& requestString)
    {
        auto request = requestFactory_.create(requestString);
        if (!request) {
            return {};
        }

        auto result = request->applyToDatabase(*db_);
        return formAnswerString(std::move(result));
    }

private: // methods
    std::optional<std::string> formAnswerString(DatabaseResult result)
    {
        if (result.error()) {
            return formErrorString(result);
        } else {
            return formPositiveAnswerString(result);
        }
    }

    std::string formPositiveAnswerString(const DatabaseResult& result) {
        std::string answer;
        for(const auto& [id, values] : result){
            answer += std::to_string(id);
            for (const auto& value : values){
                answer += ',';
                answer += value;
            }
            answer += '\n';
        }
        answer += "OK";

        return answer;
    }

    std::string formErrorString(const DatabaseResult& result)
    {
        std::string answer = "ERR " + toString(result.error().value());
        if (result.error().value() == DatabaseError::ValueAlreadyExists) {
            answer += ' ';
            answer += std::to_string(result.errorData());
        }
        return answer;
    }

private: // data
    RequestFactory            requestFactory_;
    std::shared_ptr<Database> db_;
};