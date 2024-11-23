#pragma once

#include "request.h"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <string>
#include <memory>
#include <vector>

/**/
/**/
/*INSERT table id name*/
class RequestFactory
{
public:
    std::unique_ptr<Request> create(const std::string& input) { return formRequest(parseInput(input)); }

private: // methods
    std::unique_ptr<Request> formRequest(std::vector<std::string> tokens)
    {
        if (tokens.empty()) {
            return nullptr;
        }

        if (tokens.front() == "INSERT") {
            if (tokens.size() < 4) {
                return nullptr;
            }
            auto request = std::make_unique<RequestInsert>();
            try {
                if (tokens[1] == "A") {
                    request->tableId_ = 1;
                } else if (tokens[1] == "B") {
                    request->tableId_ = 2;
                } else {
                    request->tableId_ = 0;
                }
                request->id_ = boost::lexical_cast<int>(tokens[2]);
            } catch (const std::exception&) {
                return nullptr;
            }
            request->value_ = std::move(tokens[3]);

            return std::move(request);
        } else if (tokens.front() == "TRUNCATE") {
            if (tokens.size() < 2) {
                return nullptr;
            }
            auto request = std::make_unique<RequestClear>();
            try {
                request->tableId_ = boost::lexical_cast<std::size_t>(tokens[1]);
            } catch (const std::exception&) {
                return nullptr;
            }

            return std::move(request);
        } else if (tokens.front() == "INTERSECTION") {
            if (tokens.size() < 1) {
                return nullptr;
            }
            auto request = std::make_unique<RequestIntersection>();

            return std::move(request);
        } else if (tokens.front() == "SYMMETRIC_DIFFERENCE") {
            if (tokens.size() < 1) {
                return nullptr;
            }
            auto request = std::make_unique<RequestSymmetricDifference>();

            return std::move(request);
        }

        return nullptr;
    }

    std::vector<std::string> parseInput(const std::string& input)
    {
        std::vector<std::string> tokens;
        boost::split(tokens, input, boost::is_any_of(" "));

        return tokens;
    }
};