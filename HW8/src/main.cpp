#include <boost/program_options.hpp>
#include <boost/version.hpp>

#include "cryptopp/cryptlib.h"

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
    namespace  po= boost::program_options;
    po::options_description description("Allowed options");
    description.add_options()("help", "produce help message")("compression", po::value<int>(), "set compression level");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, description), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << description << "\n";
        return 1;
    }

    if (vm.count("compression")) {
        std::cout << "Compression level was set to " << vm["compression"].as<int>() << ".\n";
    } else {
        std::cout << "Compression level was not set.\n";
    }

    std::cout << "Using Boost " << BOOST_VERSION / 100000 << "." // major version
              << BOOST_VERSION / 100 % 1000 << "."               // minor version
              << BOOST_VERSION % 100                             // patch level
              << std::endl;

    std::string msg1 = "Lorem ipsum dolor sit amet conse";
    std::string msg2 = "Lorem ipsum dolor sit amet consectetur adipisicing elit. Enim,";
    std::string msg3 = "Lorem ipsum dolor sit amet consectetur adipisicing elit. Enim, neque exercitationem? Et iusto "
                       "veniam nostrum voluptatem dolor, m";
    std::string msg4 = "Lorem ipsum dolor sit amet consectetur adipisicing elit. Enim, neque exercitationem? Et iusto "
                       "veniam nostrum voluptatem dolor, maxime deleniti harum aperiam molestias animi quam assumenda "
                       "ipsam repellat earum ab quae. Lorem ipsum dolor sit amet consectetur";
    std::string msg5 =
        "Lorem ipsum dolor sit amet consectetur adipisicing elit. Enim, neque exercitationem? Et iusto veniam nostrum "
        "voluptatem dolor, maxime deleniti harum aperiam molestias animi quam assumenda ipsam repellat earum ab quae. "
        "Lorem ipsum dolor sit amet consectetur adipisicing elit. Enim, neque exercitationem? Et iusto veniam nostrum "
        "voluptatem dolor, maxime deleniti harum aperiam molestias animi quam assumenda ipsam repellat earum ab quae. "
        "Lorem ipsum dolor sit amet consectetur adipisicing elit. Enim, neque exerci";
    std::string digest;

        digest = sha3(msg1);
    std::cout << "Message: " << msg1 << std::endl;
    std::cout << "Digest: " << digest << std::endl;

    return 0;
}
