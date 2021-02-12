#include "tasker.h"
#include "scenaries.h"
#include "map.h"
#include "mapf_interface.h"
#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "Too few arguements\n";
        return 1;
    }

    std::cout << "Preparing map from " << argv[1] << "\n";

    Map map;
    ScenariesCollection scens;

    // Load a map
    std::ifstream file(argv[1]);
    if (!file.is_open())
    {
        std::cout << "Error occured while trying to open a file with map\n";
        return 1;
    }

    map.PrepareMap(file);

    std::cout << "Map size is " << map.GetHeight() << " " << map.GetWidth() << "\n";

    file.close();
    // Finish loading the map

    // Load scenaries
    file.open(argv[2]);
    if (!file.is_open())
    {
        std::cout << "Error occured while trying to open a file with scenaries\n";
        return 1;
    }

    scens.PrepareScenaries(file);

    std::cout << "Found " << scens.GetNum() << " scenaries\n";

    file.close();
    // Finish loading scenaries

    return 0;
}
