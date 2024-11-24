#include <iostream>
#include <string>

int main() {
    std::string line;
    int counter = 1;
    while(std::getline(std::cin, line) && ++counter < 10){
        std::cout << "source " << counter << '\n';
    }

    return 0;
}