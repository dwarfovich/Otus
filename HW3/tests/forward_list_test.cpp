#include "cc/forward_list.h"

#include <gtest/gtest.h>

TEST(ForwardListTest, ConstructingEmpty)
{
    cc::ForwardList<int> list;
    EXPECT_TRUE(list.empty());
}

TEST(ForwardListTest, InsertionAfterInBeginning)
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
    const int inserts = 4;
    
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