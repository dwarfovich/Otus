#include "map_reduce/data_extractor.h"
#include "map_reduce/map_functions.h"

#include <limits>
#include <iostream>
#include <string>

int main() {
    std::string line;
    std::size_t sum = 0;
    std::size_t lineCounter = 0;
    while (std::getline(std::cin, line) && ++lineCounter < 10) {
        const auto price = extractUll(line, priceValueNumber);
        if(!price.has_value()){
            std::cerr << "Unable to extract price from line " << lineCounter << '\n';
        } else{
            bool success = addToVar(price.value(), sum);
            if (success){
                std::cout << sum << '\n';   
            } else{
                std::cerr << "Maximum sum reached - exiting\n";
                return -1;
            }
        }
    }

    return 0;
}