#include "map_reduce/reduce_functions.h"

#include <iostream>
#include <string>
#include <cinttypes>
#include <format>

int main()
{
    double      inputCounter = 0;
    std::size_t totalPrice   = 0;
    while (std::cin >> totalPrice) {
        if (!std::cin.good()) {
            std::cerr << "Failed to read input - skipping it\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        ++inputCounter;
        std::cout << std::format("{:.3f}\n", totalPrice / inputCounter);

       if(!canSafelyIncrementDouble(inputCounter)){
            std::cerr << "Maximum inputs reached - exiting\n";
            return -1;
       }
    }

    return 0;
}