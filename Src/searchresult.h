#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H
#include <list>
#include "node.h"

//That's the output structure for the search algorithms.
//The task of the pathfinder is to fill it in a right way.

//Practically, after SearchResult is filled by the path finding algorithm,
//one may invoke XMLogger methods like writeToLogSummary etc.
//(passing appropriate fields of SearchResult as parameters)
//and these methods will flush the data to the XML.

//lppath is a path that is a sequence of nodes, corresponding to adjacent cells on the grid.

//hppath is a path that is a sequence of pivotal nodes that may not be adjacent on the grid.
//It's like a path composed of the sections.

//You need to fill BOTH sorts of path (so both lppath and hppath will be flushed to output log).

// A*/Dijkstra naturally generates lppath. One might post-process it by skipping the nodes at which
//the movement direction does not change to form sections of hppath.

struct SearchResult
{
        bool pathfound;
        float pathlength; //if path not found, then pathlength=0
        const std::list<Node>* lppath; //path as the sequence of adjacent nodes (see above)
                                       //This is a pointer to the list of nodes that is actually created and hadled by Search class,
                                       //so no need to re-create them, delete them etc. It's just a trick to save some memory
        const std::list<Node>* hppath; //path as the sequence of non-adjacent nodes: "sections" (see above)
                                        //This is a pointer to the list of nodes that is actually created and hadled by Search class,
                                        //so no need to re-create them, delete them etc. It's just a trick to save some memory
        unsigned int nodescreated; //|OPEN| + |CLOSE| = total number of nodes saved in memory during search process.
        unsigned int numberofsteps; //number of iterations made by algorithm to find a solution
        double time; //runtime of the search algorithm (expanding nodes + reconstructing the path)
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

#endif
