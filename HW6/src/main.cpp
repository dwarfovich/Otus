﻿#include "matrix/matrix.h"

#include <iostream>

int main()
{
    SparseMatrix<int, 2> m(-1);

    auto t = m[0][3];
    try {
        t = 5;
    } catch (const std::exception& e) {
        std::cout << "!!! t = 5;" << '\n';
        std::cout << "Exception! " << e.what() << '\n';
    }
    const auto ct = m[4][432];
    // ct = 5;

    try {
        m[123] = 43;
    } catch (const std::exception& e) {
        std::cout << "!!! m[123] = 43;" << '\n';
        std::cout << "Exception! " << e.what() << '\n';
    }

    m[55][3663] = 25;
    m[55][534]  = 888;

    std::cout << "\nIterating:" << std::endl;
    for (const auto& c : m) {
        std::cout << c << std::endl;
    }

    return 0;
}
