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
#include "config.h"

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

    unsigned int size();
};


class Search
{
    public:
        Search();
        ~Search(void);
        SearchResult startSearch(ILogger *Logger, const Map &Map, const EnvironmentOptions &options, const Config& config);

    protected:
        // Search parametres.
        bool isDijk;
        int maxSize;
        int task[4]; // 0 - start_i, 1 - start_j, 2 - goal_i, 3 - goal_j;
        EnvironmentOptions currentOptions;

        // Search resources.
        NodesBinaryHeap openHeap;
        std::unordered_map<int, Node> generatedNodes;
        
        // Results.
        SearchResult sresult; 
        std::list<Node> lppath, hppath;

    protected:
        int encode(int x, int y, int maxValue);

        virtual void setHeuristic(Node& nodeToEdit);

        void expandNode(Node* nodeToExpand, const Map &map);
};
#endif
