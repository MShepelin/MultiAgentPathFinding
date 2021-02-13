#include "map.h"
#include <gtest/gtest.h>

#define EPS 1e-5

TEST(MapTest, Construction)
{
    Map map;

    ASSERT_EQ(map.GetWidth(), 0);
    ASSERT_EQ(map.GetHeight(), 0);
}

#undef EPS