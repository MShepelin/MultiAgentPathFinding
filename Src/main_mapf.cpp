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

    tasker.GetSolver()->SetDepth(10);
    tasker.SetOptions({ true, 3 });

    std::cout << "Print \"1 x\" to solve scenario x\n";
    std::cout << "Print \"2 x, y\" to solve scenarios x-y\n";
    std::cout << "Print \"depth x\" to set new depth of cooperation\n";

    int x, y;
    std::string command;
    while (std::cin >> command)
    {
        if (command == "1")
        {
            if (std::cin >> x)
            {
                tasker.StartSearch(x, x, &std::cout);
                continue;
            }
        }
        else if (command == "2")
        {
            if (std::cin >> x >> y)
            {
                tasker.StartSearch(x, y, &std::cout);
                continue;
            }
        }
        else if (command == "depth")
        {
            if (std::cin >> x && x > 0)
            {
                tasker.GetSolver()->SetDepth(x);
                continue;
            }
        }

        std::cout << "Wrong command\n";
    }

    return 0;
}
