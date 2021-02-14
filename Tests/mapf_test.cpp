#include "map.h"
#include <gtest/gtest.h>

#define EPS 1e-5

TEST(MapTest, Construction)
{
    Map map;

    ASSERT_EQ(map.GetWidth(), 0);
    ASSERT_EQ(map.GetHeight(), 0);
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#undef EPS