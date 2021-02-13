#pragma once

#include <list>
#include "node.h"

struct SearchResult
{
        bool pathfound;
        float pathlength; 
        const std::list<Node>* lppath;
        const std::list<Node>* hppath; 
        unsigned int nodescreated; 
        unsigned int numberofsteps; 
        double time;

        SearchResult()
        {
            pathfound = false;
            pathlength = 0;
            lppath = nullptr;
            hppath = nullptr;
            nodescreated = 0;
            numberofsteps = 0;
            time = 0;
        }
};