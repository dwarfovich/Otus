#include "map_reduce/data_extractor.h"
#include "map_reduce/map_functions.h"
#include "map_reduce/reduce_functions.h"

#include <gtest/gtest.h>

#include <limits>

TEST(ExtractUll, EmptyString)
{
    auto result = extractUll("", 0);
    ASSERT_FALSE(result.has_value());

    result = extractUll("", 1);
    ASSERT_FALSE(result.has_value());
}

TEST(ExtractUll, SingleWrongValue)
{
    auto result = extractUll("a", 0);
    ASSERT_FALSE(result.has_value());

    result = extractUll(" ", 1);
    ASSERT_FALSE(result.has_value());
}

TEST(ExtractUll, SingleValue)
{
    auto result = extractUll("1", 0);
    ASSERT_EQ(result.value(), 1);

        result = extractUll("123", 0);
    ASSERT_EQ(result.value(), 123);
}

TEST(ExtractUll, SingleValueWrongIndex)
{
    auto result = extractUll("1", 1);
    ASSERT_FALSE(result.has_value());

        result = extractUll("123", 1);
    ASSERT_FALSE(result.has_value());
}

TEST(ExtractUll, SingleValueEndsWithComma)
{
    auto result = extractUll("1,", 0);
    ASSERT_EQ(result.value(), 1);

    result = extractUll("123,", 0);
    ASSERT_EQ(result.value(), 123);
}

TEST(ExtractUll, TwoValues)
{
    auto result = extractUll("1,2", 0);
    ASSERT_EQ(result.value(), 1);

    result = extractUll("1,2", 1);
    ASSERT_EQ(result.value(), 2);

    result = extractUll("123,234", 0);
    ASSERT_EQ(result.value(), 123);

    result = extractUll("123,234", 1);
    ASSERT_EQ(result.value(), 234);
}

TEST(ExtractUll, ThreeValuesWithText)
{
    auto result = extractUll("1111,2,abc", 0);
    ASSERT_EQ(result.value(), 1111);

    result = extractUll("abc def,22222, as", 1);
    ASSERT_EQ(result.value(), 22222);

    result = extractUll("abc def,2, 333333", 2);
    ASSERT_EQ(result.value(), 333333);
}

TEST(MapFunctions, AddToVarSuccess)
{
    std::size_t sum = 0;

    ASSERT_TRUE(addToVar(1, sum));
    ASSERT_EQ(sum, 1);

    ASSERT_TRUE(addToVar(2, sum));
    ASSERT_EQ(sum, 3);

    ASSERT_TRUE(addToVar(0, sum));
    ASSERT_EQ(sum, 3);
}

TEST(MapFunctions, AddToVarOverflow)
{
    std::size_t sum = std::numeric_limits<std::size_t>::max();

    ASSERT_FALSE(addToVar(1, sum));
    ASSERT_EQ(sum, std::numeric_limits<std::size_t>::max());

    sum = std::numeric_limits<std::size_t>::max() - 1;
    ASSERT_FALSE(addToVar(10, sum));
    ASSERT_EQ(sum, std::numeric_limits<std::size_t>::max() - 1);
}

TEST(ReduceFuncitons, SafelyIncrementDouble)
{
    ASSERT_TRUE(canSafelyIncrementDouble(std::numeric_limits<double>::lowest()));
    ASSERT_TRUE(canSafelyIncrementDouble(std::numeric_limits<double>::min()));
    ASSERT_TRUE(canSafelyIncrementDouble(-1.));
    ASSERT_TRUE(canSafelyIncrementDouble(0.));
    ASSERT_TRUE(canSafelyIncrementDouble(1.));
}

TEST(ReduceFuncitons, CannotSafelyIncrementDouble)
{
    ASSERT_FALSE(canSafelyIncrementDouble(std::numeric_limits<double>::infinity()));
    ASSERT_FALSE(canSafelyIncrementDouble(std::numeric_limits<double>::max()));
}