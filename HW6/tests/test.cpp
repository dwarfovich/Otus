#include "matrix/matrix.h"

#include <gtest/gtest.h>

TEST(MatrixTest, CompilationCheck)
{
    SparseMatrix<int, 4> m;

    auto t = m[4][432][1212][12300];
    ASSERT_EQ(t, m.defaultElement());
    ASSERT_EQ(m.defaultElement(), t);
    const auto ct = m[4][432][1212][12300];
    ASSERT_EQ(ct, m.defaultElement());
    ASSERT_EQ(m.defaultElement(), ct);
    const auto& crt = m[4][432][1212][12300];
    ASSERT_EQ(crt, m.defaultElement());
    ASSERT_EQ(m.defaultElement(), crt);

    m[55][3663][23][2323]               = 25;
    m[55][534][22][0]                = 888;
    ((m[100][100][0][0] = 314) = 0) = 217;

    for (const auto& c : m) {
        decltype(m)::Position position;
        decltype(m)::Element  element;
        std::tie(position, element) = c;
    }
    for (auto c : m) {
        decltype(m)::Position position;
        decltype(m)::Element  element;
        std::tie(position, element) = c;
    }
}

TEST(MatrixTest, DefaultElement)
{
    for()
    SparseMatrix<int, 4> m {0};
    ASSERT_EQ(0, m.defaultElement());
    ASSERT_EQ(m.defaultElement(), t);
}