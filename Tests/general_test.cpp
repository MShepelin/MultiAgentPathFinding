#include "map.h"
#include "nodes_heap.h"
#include <vector>
#include <gtest/gtest.h>

TEST(NodesBinaryHeap, CheckTies)
{
    NodesBinaryHeap<int> heap;
    std::vector<Node<int>> check = { 
        { 8, 10, 5 },
        { 3, 3, 3 },
        { 6, 0, 8 },
        { 1, 3, 2 },
        { 0, 1, 0 },
        { 2, 2, 3 },
        { 9, 5, 10 },
        { 7, 9, 1 },
        { 5, 4, 4 },
        { 4, 5, 3 }
    };

    for (auto& i : check)
    {
        heap.Insert(i);
    }

    int counter = 0;
    while (heap.Size())
    {
        Node<int>* node = heap.PopMin();
        EXPECT_EQ(node->cell, counter);
        counter++;
    }
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
