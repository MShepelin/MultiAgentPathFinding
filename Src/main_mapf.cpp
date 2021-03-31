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
    tasker.SetOptions({ false, true, false, false, LOG_RAW_PATHS });

    std::cout << "\nPrint file path or just print \"cout\"\n";

    bool try_to_read;
    std::string name;
    std::ofstream file;
    std::ostream *output = nullptr;

    while (std::cin >> name)
    {
        if (name == "cout")
        {
            output = &std::cout;
            break;
        }
        else
        {
            file.open(name);
            if (file.is_open())
            {
                output = &file;
                break;
            }
        }

        std::cout << "Wrong name, try again\n";
    }

    if (!output) return 0;

    std::cout << "\nPrint \"1 x\" to solve scenario x\n";
    std::cout << "Print \"2 x, y\" to solve scenarios x-y\n";
    std::cout << "Print \"rad x\" to change agents radiuses in relation to a cell size (values from 0 to 0.5)\n";
    std::cout << "Print \"depth x\" to set new depth of cooperation\n";
    std::cout << "Print \"cons\" to print task conditions in the output stream: height, width, agent radius and the grid itself\n";
    std::cout << "\n";

    int x, y;
    float rad;
    std::string command;
    while (std::cin >> command)
    {
        tasker.ClearStatistics();

        if (command == "1")
        {
            if (std::cin >> x)
            {
                tasker.StartSearch(x, x, output);
                std::cout << "End of print\n";
            }
            continue;
        }
        else if (command == "2")
        {
            if (std::cin >> x >> y)
            {
                tasker.StartSearch(x, y, output);
                tasker.PrintStatistics(&std::cout);
                std::cout << "End of print\n";
            }
            continue;
        }
        else if (command == "depth")
        {
            if (std::cin >> x && x > 0)
            {
                tasker.GetSolver()->SetDepth(x);
                std::cout << "OK, new depth\n";
            }
            else
            {
                std::cout << "Wrong input\n";
            }
            continue;
        }
        else if (command == "rad")
        {
            if (std::cin >> rad && rad >= 0 && rad <= 0.5)
            {
                tasker.GetSolver()->SetRadius(rad);
                std::cout << "OK, new agent radius\n";
            }
            else
            {
                std::cout << "Wrong input\n";
            }
            continue;
        }
        else if (command == "cons")
        {
            std::cout << tasker.GetMap()->GetHeight() << " " <<
                tasker.GetMap()->GetWidth() << " " <<
                tasker.GetSolver()->GetRadius() << "\n";

            *output << tasker.GetMap()->GetHeight() << " " <<
                tasker.GetMap()->GetWidth() << " " <<
                tasker.GetSolver()->GetRadius() << "\n";
            std::vector<GridCell> start_locs; // empty vector
            tasker.PrintMap(start_locs, output);

            std::cout << "Map print finished\n";
            continue;
        }

        std::cout << "Wrong command\n";

        // TODO skip input untill next line
    }

    return 0;
}
