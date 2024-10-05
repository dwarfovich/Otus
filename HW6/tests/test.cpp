#include "matrix/matrix.h"

#include <gtest/gtest.h>

TEST(MatrixTest, CompilationCheck)
{
    SparseMatrix<int, 4> m;

    auto t = m[4][432][1212][12300];
    ASSERT_EQ(t, m.defaultElement());
    ASSERT_EQ(m.defaultElement(), t);
    /*const auto ct = m[4][432][1212][12300];
    ASSERT_EQ(ct, m.defaultElement());
    ASSERT_EQ(m.defaultElement(), ct);
    const auto& crt = m[4][432][1212][12300];
    ASSERT_EQ(crt, m.defaultElement());
    ASSERT_EQ(m.defaultElement(), crt);*/

    m[55][3663][23][2323]           = 25;
    m[55][534][22][0]               = 888;
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

TEST(MatrixTest, CreatedEmpty)
{
    SparseMatrix<int, 777> m { 0 };
    ASSERT_TRUE(m.empty());
    SparseMatrix<int, 1> m2 { 1 };
    ASSERT_TRUE(m2.empty());
}

TEST(MatrixTest, DefaultElement)
{
    for (auto i : { -999, -1, 0, 999 }) {
        SparseMatrix<int, 777> m { i };
        ASSERT_EQ(i, m.defaultElement());
    }
}

TEST(MatrixTest, AccessOperator1Dimension)
{
    SparseMatrix<std::size_t, 1> m { 99999 };
    m[0] = 9;
    ASSERT_EQ(m[0], 9);
    m[999] = 1;
    ASSERT_EQ(m[999], 1);
    ASSERT_EQ(m.size(), 2);
}

TEST(MatrixTest, AccessOperator2Dimension)
{
    SparseMatrix<std::size_t, 2> m { 999999 };
    m[0][1] = 9;
    ASSERT_EQ(m[0][1], 9);
    m[999][999] = 1;
    ASSERT_EQ(m[999][999], 1);
    m[99999][999] = 3;
    ASSERT_EQ(m[99999][999], 3);
    m[1][0] = 2;
    ASSERT_EQ(m[1][0], 2);
    m[1][99999] = 4;
    ASSERT_EQ(m[1][99999], 4);
    ASSERT_EQ(m.size(), 5);
}

TEST(MatrixTest, RemoveElements)
{
    SparseMatrix<std::size_t, 2> m { 1 };
    m[0][1] = 1;
    ASSERT_TRUE(m.empty());

    m[99][99] = 1;
    ASSERT_TRUE(m.empty());

    m[1][1] = 0;
    m[1][1] = 1;
    ASSERT_TRUE(m.empty());

    for (std::size_t i = 0; i < 10; ++i) {
        for (std::size_t j = 0; j < 10; ++j) {
            m[i][j] = i + j;
            m[i][j] = 1;
        }
    }
    ASSERT_TRUE(m.empty());
}

TEST(MatrixTest, CanonicalAssignmentOperator)
{
    SparseMatrix<std::size_t, 2> m { 1 };

    ((m[0][0] = 314) = 0) = 217;
    ASSERT_EQ(m[0][0], 217);
    ((m[0][0] = 1) = 2) = 300;
    ASSERT_EQ(m[0][0], 300);

    ((m[1][1] = 314) = 0) = 217;
    ASSERT_EQ(m[1][1], 217);
    ((m[1][1] = 1) = 2) = 300;
    ASSERT_EQ(m[1][1], 300);

    ASSERT_EQ(m.size(), 2);
}

TEST(MatrixTest, WrongSubscriptThrows)
{
    SparseMatrix<std::size_t, 2> m { 1 };

    EXPECT_THROW(m[1] = 1, std::exception);
    EXPECT_THROW(m[1][2][3] = 1, std::exception);
}

TEST(MatrixTest, CopyConstructedProxyThrowOnAssignment)
{
    SparseMatrix<std::size_t, 2> m { 1 };

    auto t = m[1][2];
    EXPECT_THROW(t = 354, std::exception);
}