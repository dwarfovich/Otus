#include "matrix/matrix.h"

#include <iostream>
#include <iomanip>
#include <cassert>

int main()
{
    SparseMatrix<int, 2> m(-1);

    auto t = m[0][3];

    std::cout << t << '\n';
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
    ((m[100][100] = 314) = 0) = 217;

    std::cout << "\nIterating:" << std::endl;
    for (const auto& c : m) {
        decltype(m)::Position position;
        decltype(m)::Element element;
        std::tie(position, element) = c;

        printPosition(std::cout, position);
        std::cout << ": " << element << std::endl;
    }

    std::cout << '\n';
    SparseMatrix<int> matrix { -1 };
    assert(matrix.size() == 0); // все ячейки свободны
    auto a = matrix[0][0];
    assert(a == -1);
    assert(matrix.size() == 0);
    matrix[100][100] = 314;
    assert(matrix[100][100] == 314);
    assert(matrix.size() == 1);
    for (const auto& c : matrix) {
        decltype(matrix)::Position position;
        decltype(matrix)::Element  element;
        std::tie(position, element) = c;

        printPosition(std::cout, position);
        std::cout << ": " << element << std::endl;
    }

    std::cout << '\n';
    std::cout << "Homework\n";
    SparseMatrix<int> mmm { 0 };
    for (std::size_t i = 0 ; i < 10; ++i){
        mmm[i][i] = i;
        mmm[i][9-i] = 10-1 - i;
    }
    for (std::size_t i = 1; i < 9; ++i) {
        for (std::size_t j = 1; j < 9; ++j) {
            std::cout << std::setw(2) <<mmm[i][j].element() << "  ";
        }
        std::cout << '\n';
    }

    std::cout <<"Elements count: " << mmm.size() << '\n';
    std::cout << "Elements: \n";
    for (const auto& c : mmm) {
        decltype(mmm)::Position position;
        decltype(mmm)::Element  element;
        std::tie(position, element) = c;

        printPosition(std::cout, position);
        std::cout << ": " << element << std::endl;
    }

    return 0;
}
