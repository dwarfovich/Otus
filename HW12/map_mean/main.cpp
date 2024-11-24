#include "map_reduce/data_extractor.h"
#include "map_reduce/map_functions.h"

#include <limits>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    char*          argumentEnd      = nullptr;
    const unsigned targetColumnNumber = (argc == 2 ? std::strtoul(argv[1], &argumentEnd, 10) : priceColumnNumber);
    if (errno == ERANGE) {
        std::cerr << "Unable to parse argument\n";
        return -2;
    }

    std::string line;
    std::size_t sum         = 0;
    std::size_t lineCounter = 0;
    while (std::getline(std::cin, line)) {
        const auto price = extractUll(line, targetColumnNumber);
        if (!price.has_value()) {
            std::cerr << "Unable to extract price from line " << lineCounter << '\n';
        } else {
            bool success = addToVar(price.value(), sum);
            if (success) {
                std::cout << sum << '\n';
            } else {
                std::cerr << "Maximum sum reached - exiting\n";
                return -1;
            }
        }
    }

    return 0;
}