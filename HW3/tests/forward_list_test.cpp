#include "cc/forward_list.h"

#include <gtest/gtest.h>

TEST(ForwardListTest, ConstructingEmpty)
{
    cc::ForwardList<int> list;
    EXPECT_TRUE(list.empty());
}

TEST(ForwardListTest, InsertionBeforeBegin)
{
    cc::ForwardList<int> list;
    list.insert_after(list.before_begin(), 1);
    EXPECT_EQ(*list.begin(), 1);

    list.insert_after(list.before_begin(), 2);
    EXPECT_EQ(*list.begin(), 2);

    list.insert_after(list.before_begin(), 3);
    EXPECT_EQ(*list.begin(), 3);
}

TEST(ForwardListTest, InsertionAfterBegin)
{
    cc::ForwardList<int> list;
    list.insert_after(list.begin(), 1);
    EXPECT_EQ(*list.begin(), 1);

    list.insert_after(list.begin(), 2);
    EXPECT_EQ(*std::next(list.begin()), 2);

    list.insert_after(list.begin(), 3);
    EXPECT_EQ(*std::next(list.begin()), 3);
}

TEST(ForwardListTest, ConsequtiveInsertion)
{
    const int inserts = 5;
    
    cc::ForwardList<int> list;
    auto iter = list.begin();
    for (int i = 0; i < inserts; ++i){
        iter = list.insert_after(iter, i);
    }

    iter = list.begin();
    for (int i = 0; iter != list.end(); ++i, ++iter) {
        EXPECT_EQ(*iter, i);
    }
}

TEST(ForwardListTest, Size)
{
    const int inserts = 5;

    cc::ForwardList<int> list;
    auto                 iter = list.begin();
    for (int i = 0; i < inserts; ++i) {
        iter = list.insert_after(iter, i);
        EXPECT_EQ(list.size(), static_cast<std::size_t>(i + 1));
    }

    iter = list.begin();
    for (int i = 0; iter != list.end(); ++i, ++iter) {
        EXPECT_EQ(*iter, i);
    }
}