#pragma once

#include "digester.h"

#include <cryptopp/cryptlib.h>
#include <cryptopp/sha3.h>
#include <cryptopp/hex.h>

namespace b_cryptocpp {

class DigesterSha3 : public Digester
{
    std::unique_ptr<Digester> clone() const override { return std::make_unique<DigesterSha3>(*this); }
    std::string               calculate(const std::string_view& message) const override
    {
        CryptoPP::SHA3_256 hash;

        std::string input {message.cbegin(), message.cend()};
        std::string digest;
        std::string output;

        CryptoPP::StringSource(input, true, new CryptoPP::HashFilter(hash, new CryptoPP::StringSink(digest)));

        CryptoPP::StringSource(digest, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(output)));

        return output;
    }
};
} // namespace b_cryptocpp