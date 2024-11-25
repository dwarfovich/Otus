#include "map_reduce/reduce_functions.h"

#include <iostream>
#include <string>
#include <cinttypes>
#include <format>

int main()
{
    std::size_t sum  = 0;
    std::size_t sumSquared = 0;

    double      inputCounter = 0.;
    while (std::cin >> sum >> sumSquared) {
        if (!std::cin.good()) {
            std::cerr << "Failed to read input - skipping it\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        ++inputCounter;

        double dispersion;
        const auto success = calculateDispersion(inputCounter, sum, sumSquared, dispersion);
        if(success){
            std::cout << std::format("{:.3f}\n", dispersion);
        } else{
            std::cout << "ERROR\n";
            std::cerr << "Failed to calculate dispersion\n";
        }

        if (!canSafelyIncrementDouble(inputCounter)) {
            std::cerr << "Maximum inputs reached - exiting\n";
            return -1;
        }
    }

    return 0;
}