#include "cc/forward_list.h"

#include <gtest/gtest.h>

#include <forward_list>
TEST(ForwardListTest, ConstructingEmpty)
{
    cc::ForwardList<int> list;
    EXPECT_TRUE(list.empty());
}