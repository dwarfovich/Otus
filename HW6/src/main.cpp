#include "matrix/matrix.h"

#include <iostream>

int main()
{
    SparseMatrix<int, 2> m (-1);
    auto t = m[0][3];
    try{
        t = 5;
    } catch(const std::exception& e){
        std::cout << "!!! t = 5;" <<'\n';
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

    

    //t = 5;
   //m[0][1] = 45;
   //int tint = m[0][0];

    return 0;
}
