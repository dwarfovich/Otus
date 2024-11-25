#pragma once

#include <string>
#include <memory>
#include <optional>

class RequestProcessor
{
public:
    std::optional<std::string> process(const std::string& requestString)
    {
    /*    auto request = requestFactory_.create(requestString);
        if (!request) {
            return {};
        }

        auto result = request->applyToDatabase(*db_);
        return formAnswerString(std::move(result));*/
        return {};
    }

private: // methods
    std::optional<std::string> formAnswerString()
    {
        /*if (result.error()) {
            return formErrorString(result);
        } else {
            return formPositiveAnswerString(result);
        }*/
        return {};
    }

    std::string formPositiveAnswerString() {
        /*std::string answer;
        for(const auto& [id, values] : result){
            answer += std::to_string(id);
            for (const auto& value : values){
                answer += ',';
                answer += value;
            }
            answer += '\n';
        }
        answer += "OK";

        return answer;*/
        return {};
    }

    std::string formErrorString()
    {
        /*std::string answer = "ERR " + toString(result.error().value());
        if (result.error().value() == DatabaseError::ValueAlreadyExists) {
            answer += ' ';
            answer += std::to_string(result.errorData());
        }
        return answer;*/
        return {};
    }

private: // data
};