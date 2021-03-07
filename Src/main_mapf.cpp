#include <cassert>
#include "tasker.h"
#include "mapf/ma_star.h"
#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cout << "Too few arguements\n";
        return 1;
    }

    Tasker tasker;
    if (!tasker.PrepareMap(argv[1]))
    {
        std::cout << "Wrong map!\n";
        return 1;
    }

    std::cout << "Map file: " << argv[1] << "\n";

    if (!tasker.PrepareConfig(argv[2]))
    {
        std::cout << "Wrong configuration file!\n";
        return 1;
    }

    std::cout << "Configuration file: " << argv[2] << "\n";
    
    if (!tasker.PrepareScenaries(argv[3]))
    {
        std::cout << "Wrong file with scenaries!\n";
        return 1;
    }

    std::cout << "Scenaries file: " << argv[3] << "\n";

    if (!tasker.GetScenariesNum())
    {
        std::cout << "No scenaries loaded\n";
    }

    //tasker.StartSearch(0, &std::cout);
    //SearchResult<GridCell> sr = astar_solver.GetPlan(0);

    tasker.GetSolver()->SetDepth(10);
    tasker.StartSearch(0, tasker.GetScenariesNum() - 1, &std::cout);

    return 0;
}
