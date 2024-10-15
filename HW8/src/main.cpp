#include "duplicate_finder/Header1.h"

#include <boost/program_options.hpp>
#include <boost/version.hpp>

#include <cryptopp/cryptlib.h>
#include <cryptopp/sha3.h>
#include <cryptopp/hex.h>

#include <iostream>


std::string sha3(std::string& input)
{
    CryptoPP::SHA3_256 hash;

    std::string digest;
    std::string output;

    CryptoPP::StringSource(input,
                           true,
                           new CryptoPP::HashFilter(hash,
                                                    new CryptoPP::StringSink(digest)) // HashFilter
    );                                                                                // StringSource

    CryptoPP::StringSource(digest, true,
                           new CryptoPP::HexEncoder(new CryptoPP::StringSink(output))); // StringSource

    return output;
}

int main(int argc, char* argv[])
{

    return 0;
}
