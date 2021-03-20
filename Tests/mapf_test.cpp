#include "map.h"
#include "mapf/whca.h"
#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include "tasker.h"

#define EPS 1e-5

#ifndef TEST_DIR
#define TEST_DIR "error string"
#endif

TEST(MapTest, Construction)
{
    Map map;

    ASSERT_EQ(map.GetWidth(), 0);
    ASSERT_EQ(map.GetHeight(), 0);
}

TEST(WHCATest, Contruction)
{
    Tasker tasker;
    ASSERT_TRUE(tasker.PrepareMap(TEST_DIR "\\test1.map"));
    ASSERT_TRUE(tasker.PrepareConfig(TEST_DIR "\\BaseConfig.xml"));
    ASSERT_TRUE(tasker.PrepareScenaries(TEST_DIR "\\test1.scen"));
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#undef EPS