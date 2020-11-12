#ifndef SEARCH_H
#define SEARCH_H
#include "ilogger.h"
#include "searchresult.h"
#include "environmentoptions.h"
#include <list>
#include <vector>
#include <math.h>
#include <limits>
#include <chrono>
#include <unordered_map>

class NodesBinaryHeap
{
private:
    std::vector<Node*> nodes;

    void moveUp(size_t nodeIndex);
    void moveDown(size_t nodeIndex);

public:
    NodesBinaryHeap();

    void insert(Node& newNode);

    void decreaseGValue(Node& nodeToChange, double newGValue);

    Node* popMin();
};

class Search
{
    public:
        Search();
        ~Search(void);
        SearchResult startSearch(ILogger *Logger, const Map &Map, const EnvironmentOptions &options);

    protected:
        //CODE HERE

        //Hint 1. You definetely need class variables for OPEN and CLOSE

        //Hint 2. It's a good idea to define a heuristic calculation function, that will simply return 0
        //for non-heuristic search methods like Dijkstra
        

        //Hint 3. It's a good idea to define function that given a node (and other stuff needed)
        //will return it's sucessors, e.g. unordered list of nodes

        //Hint 4. working with OPEN and CLOSE is the core
        //so think of the data structures that needed to be used, about the wrap-up classes (if needed)
        //Start with very simple (and ineffective) structures like list or vector and make it work first
        //and only then begin enhancement!

        SearchResult                    sresult; //This will store the search result
        std::list<Node>                 lppath, hppath; //

        //CODE HERE to define other members of the class
};
#endif
