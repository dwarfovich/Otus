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
}

TEST(ForwardListTest, EraseBegin)
{
    const int inserts = 5;

    cc::ForwardList<int> list;
    auto                 iter = list.begin();
    for (int i = 0; i < inserts; ++i) {
        iter = list.insert_after(iter, i);
    }

    for (int i = 0; i < inserts; ++i) {
        EXPECT_EQ(*list.begin(), i);
        list.erase_after(list.before_begin());
    }
    EXPECT_TRUE(list.empty());
}

TEST(ForwardListTest, EraseInMiddle)
{
    const int inserts = 11;

    cc::ForwardList<int> list;
    auto                 iter = list.begin();
    for (int i = 0; i < inserts; ++i) {
        iter = list.insert_after(iter, i);
    }

    iter = list.begin();
    std::advance(iter, inserts/2);
    for (int i = inserts - inserts/2; i < inserts - 1; ++i) {
        EXPECT_EQ(*iter, inserts - inserts / 2 - 1);
        EXPECT_EQ(*std::next(iter), i );
        list.erase_after(iter);
    }

    auto t = list.cbegin();
    iter == t;
}