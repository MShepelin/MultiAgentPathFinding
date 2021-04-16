#include <cassert>
#include "tasker.h"
#include <iostream>
#include <string>
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
        std::cout << "Wrong file with scenarios!\n";
        return 1;
    }

    std::cout << "Scenarios file: " << argv[3] << "\n";

    if (!tasker.GetScenariesNum())
    {
        std::cout << "No scenaries loaded\n";
    }

    tasker.SetOptions({ false, true, false, false, NO_PLAN_LOG });

    std::cout << "\nPrint file path\n";

    bool try_to_read;
    std::string name;
    std::ofstream file;

    while (std::cin >> name)
    {
        file.open(name);
        if (file.is_open())
        {
            break;
        }

        std::cout << "Wrong name, try again\n";
    }

    if (!file.is_open()) return 0;

    std::string stats_name;
    //std::getline(std::cin, stats_name);

    int start_depth = 8, end_depth = 80, delta_depth = 4, tries = 5;

    // TODO: check if start and end are right
    int start, end;
    std::cin >> stats_name >> start >> end;

    file << stats_name << "[" << start << ":" << end << "]";

    for (int depth = start_depth; depth < end_depth; depth += delta_depth)
    {
        file << ";" << depth;
    }

    file << "\n";

    for (float radius = 0.1; radius < 0.425; radius += 0.1)
    {
        file << radius;

        std::cout << radius << ": ";

        for (int depth = start_depth; depth < end_depth; depth += delta_depth)
        {
            tasker.GetSolver()->SetDepth(depth);

            double sum = 0;
            int pathfound = 0;

            for (int i = 0; i < tries; ++i)
            {
                tasker.StartSearch(start, end, &file);

                const std::vector<Stats>* all_res = tasker.GetStatistics();
                Stats res = all_res->at(0);
                tasker.ClearStatistics();

                if (res.path_found) pathfound++;
                sum += res.total_time;

                std::cout << "|";
            }

            std::cout << ":";

            file << ";" << sum / tries << "(" << pathfound << "/" << tries << ")";
        } 

        std::cout << "\n";

        file << "\n";
    }

    return 0;
}
