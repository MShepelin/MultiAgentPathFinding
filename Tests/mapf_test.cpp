#include "map.h"
#include "mapf/whca.h"
#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "tasker.h"

#ifndef TEST_DIR
#define TEST_DIR "error string"
#endif

TEST(MapTest, Construction)
{
    Map map;

    ASSERT_EQ(map.GetWidth(), 0);
    ASSERT_EQ(map.GetHeight(), 0);
}

TEST(WHCATest, SmallMap)
{
    Tasker tasker;
    ASSERT_TRUE(tasker.PrepareMap(TEST_DIR "\\0_test.map"));
    ASSERT_TRUE(tasker.PrepareConfig(TEST_DIR "\\BaseConfig.xml"));
    ASSERT_TRUE(tasker.PrepareScenaries(TEST_DIR "\\0_test.scen"));

    tasker.SetOptions({ true, false, true, false, NO_PLAN_LOG });
    tasker.StartSearch(0, 2, &std::cout);
}

TEST(WHCATest, MazeMap1)
{
    Tasker tasker;
    ASSERT_TRUE(tasker.PrepareMap(TEST_DIR "\\maze-32-32-2.map"));
    ASSERT_TRUE(tasker.PrepareConfig(TEST_DIR "\\BaseConfig.xml"));
    ASSERT_TRUE(tasker.PrepareScenaries(TEST_DIR "\\maze-32-32-2-even-1.scen"));

    std::ostringstream output;
    tasker.GetSolver()->SetDepth(20);
    tasker.SetOptions({ true, false, true, false, NO_PLAN_LOG });
    tasker.StartSearch(5, 20, &std::cout);
    for (Stats stats : *tasker.GetStatistics())
    {
        EXPECT_TRUE(stats.path_found);
        EXPECT_TRUE(stats.check);
    }
}

TEST(WHCATest, MazeMap2)
{
    Tasker tasker;
    ASSERT_TRUE(tasker.PrepareMap(TEST_DIR "\\maze-32-32-2.map"));
    ASSERT_TRUE(tasker.PrepareConfig(TEST_DIR "\\BaseConfig.xml"));
    ASSERT_TRUE(tasker.PrepareScenaries(TEST_DIR "\\maze-32-32-2-even-1.scen"));

    std::ostringstream output;
    tasker.GetSolver()->SetDepth(20);
    tasker.SetOptions({ true, false, true, false, NO_PLAN_LOG });
    tasker.StartSearch(20, 30, &std::cout);
    for (Stats stats : *tasker.GetStatistics())
    {
        EXPECT_TRUE(stats.path_found);
        EXPECT_TRUE(stats.check);
    }
}

TEST(WHCATest, BerlinMap)
{
    Tasker tasker;
    ASSERT_TRUE(tasker.PrepareMap(TEST_DIR "\\Berlin_1_256.map"));
    ASSERT_TRUE(tasker.PrepareConfig(TEST_DIR "\\BaseConfig.xml"));
    ASSERT_TRUE(tasker.PrepareScenaries(TEST_DIR "\\Berlin_1_256-even-1.scen"));

    std::ostringstream output;
    tasker.GetSolver()->SetDepth(10);
    tasker.SetOptions({ true, true, true, false, NO_PLAN_LOG });
    tasker.StartSearch(2, 42, &std::cout);
    for (Stats stats : *tasker.GetStatistics())
    {
        EXPECT_TRUE(stats.path_found);
        EXPECT_TRUE(stats.check);
    }
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
