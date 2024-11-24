#include "map_reduce/data_extractor.h"

#include <limits>
#include <iostream>
#include <string>

int main() {
    std::string line;
    std::size_t sum = 0;
    int lineCounter = 0;
    while (std::getline(std::cin, line) && ++lineCounter < 10) {
        const auto price = extractUll(line, priceValueNumber);
        if(!price.has_value()){
            std::cerr << "Unable to extract price from line " << lineCounter << '\n';
        } else{
            if ((sum + price.value()) < sum){
                std::cerr << "Maximum sum reached - exiting\n";
                return -1;
            } else{
                sum += price.value();
                std::cout << sum << '\n';   
            }

        }
    }

    return 0;
}