#include "map_reduce/data_extractor.h"

#include <gtest/gtest.h>

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