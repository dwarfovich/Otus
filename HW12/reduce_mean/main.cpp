#include <iostream>
#include <string>
#include <cinttypes>

int main()
{
    double      inputCounter = 0;
    std::size_t totalPrice   = 0;
    while (std::cin >> totalPrice) {
        ++inputCounter;
        std::cout << totalPrice / inputCounter << '\n';

        static constexpr double maxDouble = std::numeric_limits<double>::max();
        static constexpr double epsilon   = 0.000'000'1;
        using std::abs;
        using std::max;
        using std::min;
        if (abs(inputCounter - maxDouble) <= max(inputCounter, maxDouble) * epsilon) {
            std::cerr << "Maximum inputs reached - exiting\n";
            return -1;
        }
    }

    return 0;
}