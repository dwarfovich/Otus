#include "matrix/matrix.h"

#include <iostream>

int main()
{
    Matrix<int, 2> m (-1);
    auto t = m[0][3];
    t = 5;
   m[0][1] = 45;

    return 0;
}
