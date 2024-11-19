#pragma once

#include "digester.h"

#include <cryptopp/cryptlib.h>
#include <cryptopp/sha3.h>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>
#include <cryptopp/hex.h>

namespace b_cryptocpp {

class DigesterSha3 : public Digester
{
public:
    std::string calculate(const std::string_view& message) const override
    {
        CryptoPP::SHA3_256 hash;

        std::string input { message.cbegin(), message.cend() };
        std::string digest;
        std::string output;

        CryptoPP::StringSource(input, true, new CryptoPP::HashFilter(hash, new CryptoPP::StringSink(digest)));
        CryptoPP::StringSource(digest, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(output)));

        return output;
    }
    const std::string& name() const override
    {
        static const std::string name_ = "CryptoCpp-SHA3";

        return name_;
    }
};

class DigesterMd5 : public Digester
{
public:
    std::string calculate(const std::string_view& message) const override
    {
        CryptoPP::Weak::MD5 hash;

        std::string input { message.cbegin(), message.cend() };
        std::string digest;
        std::string output;

        CryptoPP::StringSource(input, true, new CryptoPP::HashFilter(hash, new CryptoPP::StringSink(digest)));
        CryptoPP::StringSource(digest, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(output)));

        return output;
    }
    const std::string& name() const override
    {
        static const std::string name_ = "CryptoCpp-MD5";

        return name_;
    }
};

} // namespace b_cryptocpp