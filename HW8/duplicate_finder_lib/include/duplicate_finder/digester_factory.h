#pragma once

#include "digester.h"
#include "cryptocpp_digesters.h"

#include <boost/algorithm/string.hpp>

class DigesterFactory
{
public:
    std::shared_ptr<Digester> createDigester(std::string algorithm)
    {
        boost::algorithm::to_lower(algorithm);
        if (algorithm == "dummy") {
            return std::make_shared<DummyDigester>();
        } else if (algorithm == "sha3") {
            return std::make_shared<b_cryptocpp::DigesterSha3>();
        } else if (algorithm == "md5") {
            return std::make_shared<b_cryptocpp::DigesterMd5>();
        } else {
            return nullptr;
        }
    }
};