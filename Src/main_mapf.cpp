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

    assert(tasker.GetScenariesNum() > 0);

    //tasker.StartSearch(0, &std::cout);
    //SearchResult<GridCell> sr = astar_solver.GetPlan(0);

    tasker.StartSearch(0, 1, &std::cout);

    /*
    std::ofstream file("res.txt");
    if (!file.is_open())
        return 0;

    tasker.PrintStatistics(&file);

    // Map paint
    const Map* map = tasker.GetMap();

    for (int i = 0; i < map->GetHeight(); ++i)
    {
        for (int j = 0; j < map->GetWidth(); ++j)
        {
            bool found = false;
            for (Node<GridCell> node : *sr.lppath)
            {
                if (node.cell.i == i && node.cell.j == j)
                {
                    file << "0";
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                if (map->IsCellTraversable(i, j))
                {
                    file << ".";
                }
                else
                {
                    file << "@";
                }
            }
        }

        file << "\n";
    }
    

    file.close();*/

    return 0;
}
