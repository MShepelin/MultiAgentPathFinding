#include "tasker.h"
#include <iostream>

Tasker::Tasker()
{
    fileName = nullptr;
}

Tasker::Tasker(const char *FileName)
{
    fileName = FileName;
}

Tasker::~Tasker()
{

}

bool Tasker::prepareMap()
{
    return true;//map.PrepareMap(fileName);
}

bool Tasker::getConfig()
{
    return config.PrepareConfig(fileName);
}

bool Tasker::createLog()
{
    return false;
}

void Tasker::createEnvironmentOptions()
{
    options.cutcorners = config.SearchParams[CN_SP_CC];
    options.allowsqueeze = config.SearchParams[CN_SP_AS];
    options.allowdiagonal = config.SearchParams[CN_SP_AD];
    options.metrictype = config.SearchParams[CN_SP_MT];
}

void Tasker::startSearch()
{
    //sr = search.startSearch(map, options, config);
}

void Tasker::printSearchResultsToConsole()
{
    std::cout << "Path ";
    if (!sr.pathfound)
        std::cout << "NOT ";
    std::cout << "found!" << std::endl;
    std::cout << "numberofsteps=" << sr.numberofsteps << std::endl;
    std::cout << "nodescreated=" << sr.nodescreated << std::endl;
    if (sr.pathfound) {
        std::cout << "pathlength=" << sr.pathlength << std::endl;
        std::cout << "pathlength_scaled=" << sr.pathlength << std::endl;
    }
    std::cout << "time=" << sr.time << std::endl;
}

void Tasker::saveSearchResultsToLog()
{
    
}

Map Tasker::getMap() const
{
    return map;
}
