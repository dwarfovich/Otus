#include "cc/forward_list.h"

#include <gtest/gtest.h>

TEST(ForwardListTest, EqualityOperatorsBeforeBeginForEmptyList)
{
    cc::ForwardList<int> list;
    EXPECT_EQ(list.before_begin(), list.before_begin());
    EXPECT_EQ(list.before_begin(), list.cbefore_begin());
    EXPECT_EQ(list.cbefore_begin(), list.before_begin());
    EXPECT_EQ(list.cbefore_begin(), list.cbefore_begin());
    EXPECT_NE(list.before_begin(), list.begin());
    EXPECT_NE(list.before_begin(), list.cbegin());
    EXPECT_NE(list.before_begin(), list.end());
    EXPECT_NE(list.before_begin(), list.cend());
    EXPECT_NE(list.cbefore_begin(), list.begin());
    EXPECT_NE(list.cbefore_begin(), list.cbegin());
    EXPECT_NE(list.cbefore_begin(), list.end());
    EXPECT_NE(list.cbefore_begin(), list.cend());
}

TEST(ForwardListTest, EqualityOperatorsBeforeBeginForNonEmptyList)
{
    cc::ForwardList<int> list;
    list.insert_after(list.cbefore_begin(), 77);
    EXPECT_EQ(list.before_begin(), list.before_begin());
    EXPECT_EQ(list.before_begin(), list.cbefore_begin());
    EXPECT_EQ(list.cbefore_begin(), list.before_begin());
    EXPECT_EQ(list.cbefore_begin(), list.cbefore_begin());
    EXPECT_NE(list.before_begin(), list.begin());
    EXPECT_NE(list.before_begin(), list.cbegin());
    EXPECT_NE(list.before_begin(), list.end());
    EXPECT_NE(list.before_begin(), list.cend());
    EXPECT_NE(list.cbefore_begin(), list.begin());
    EXPECT_NE(list.cbefore_begin(), list.cbegin());
    EXPECT_NE(list.cbefore_begin(), list.end());
    EXPECT_NE(list.cbefore_begin(), list.cend());
}

TEST(ForwardListTest, EqualityOperatorsForEmptyList)
{
    cc::ForwardList<int> list;
    EXPECT_EQ(list.begin(), list.begin());
    EXPECT_EQ(list.begin(), list.cbegin());
    EXPECT_EQ(list.cbegin(), list.begin());
    EXPECT_EQ(list.cbegin(), list.cbegin());
    EXPECT_EQ(list.begin(), list.end());
    EXPECT_EQ(list.begin(), list.cend());
    EXPECT_EQ(list.cbegin(), list.end());
    EXPECT_EQ(list.cbegin(), list.cend());
    EXPECT_EQ(list.end(), list.end());
    EXPECT_EQ(list.end(), list.cend());
    EXPECT_EQ(list.cend(), list.end());
    EXPECT_EQ(list.cend(), list.cend());
    
}

TEST(ForwardListTest, EqualityOperatorsForNonEmptyList)
{
    cc::ForwardList<int> list;
    list.insert_after(list.cbegin(), 3);
    EXPECT_EQ(list.begin(), list.begin());
    EXPECT_EQ(list.begin(), list.cbegin());
    EXPECT_EQ(list.cbegin(), list.begin());
    EXPECT_EQ(list.cbegin(), list.cbegin());
    EXPECT_NE(list.begin(), list.end());
    EXPECT_NE(list.begin(), list.cend());
    EXPECT_NE(list.cbegin(), list.end());
    EXPECT_NE(list.cbegin(), list.cend());
    EXPECT_EQ(list.end(), list.end());
    EXPECT_EQ(list.end(), list.cend());
    EXPECT_EQ(list.cend(), list.end());
    EXPECT_EQ(list.cend(), list.cend());
}

TEST(ForwardListTest, ConstructingEmpty)
{
    cc::ForwardList<int> list;
    EXPECT_TRUE(list.empty());
}

TEST(ForwardListTest, InsertionBeforeBegin)
{
    cc::ForwardList<int> list;
    list.insert_after(list.cbefore_begin(), 1);
    EXPECT_EQ(*list.cbegin(), 1);

    list.insert_after(list.cbefore_begin(), 2);
    EXPECT_EQ(*list.cbegin(), 2);

    list.insert_after(list.cbefore_begin(), 3);
    EXPECT_EQ(*list.cbegin(), 3);
}

TEST(ForwardListTest, InsertionAfterBegin)
{
    cc::ForwardList<int> list;
    list.insert_after(list.cbegin(), 1);
    EXPECT_EQ(*list.cbegin(), 1);

    list.insert_after(list.cbegin(), 2);
    EXPECT_EQ(*std::next(list.cbegin()), 2);

    list.insert_after(list.cbegin(), 3);
    EXPECT_EQ(*std::next(list.cbegin()), 3);
}

TEST(ForwardListTest, ConsequtiveInsertion)
{
    const int inserts = 5;
    
    cc::ForwardList<int> list;
    auto iter = list.cbegin();
    for (int i = 0; i < inserts; ++i){
        iter = list.insert_after(iter, i);
    }

    iter = list.cbegin();
    for (int i = 0; iter != list.end(); ++i, ++iter) {
        EXPECT_EQ(*iter, i);
    }
}

TEST(ForwardListTest, Size)
{
    const int inserts = 5;

    cc::ForwardList<int> list;
    auto                 iter = list.cbegin();
    for (int i = 0; i < inserts; ++i) {
        iter = list.insert_after(iter, i);
        EXPECT_EQ(list.size(), static_cast<std::size_t>(i + 1));
    }
}

TEST(ForwardListTest, EraseBegin)
{
    const int inserts = 5;

    cc::ForwardList<int> list;
    auto                 iter = list.cbegin();
    for (int i = 0; i < inserts; ++i) {
        iter = list.insert_after(iter, i);
    }

    for (int i = 0; i < inserts; ++i) {
        EXPECT_EQ(*list.cbegin(), i);
        list.erase_after(list.cbefore_begin());
    }
    EXPECT_TRUE(list.empty());
}

TEST(ForwardListTest, EraseInMiddle)
{
    const int inserts = 11;

    cc::ForwardList<int> list;
    auto                 iter = list.cbegin();
    for (int i = 0; i < inserts; ++i) {
        iter = list.insert_after(iter, i);
    }

    iter = list.cbegin();
    std::advance(iter, inserts/2);
    for (int i = inserts - inserts/2; i < inserts - 1; ++i) {
        EXPECT_EQ(*iter, inserts - inserts / 2 - 1);
        EXPECT_EQ(*std::next(iter), i );
        list.erase_after(iter);
    }
}