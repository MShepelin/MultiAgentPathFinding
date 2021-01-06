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

    Node* popMin();
    void insert(Node& newNode);
    void decreaseGValue(Node& nodeToChange, double newGValue);

    size_t size();
};


class Search
{
    public:
        Search();
        ~Search(void);
        SearchResult startSearch(ILogger *Logger, const Map &Map, const EnvironmentOptions &options, const Config& config);

    protected:
        NodesBinaryHeap openHeap;
        std::unordered_map<int, Node> generatedNodes;
        std::list<Node> lppath, hppath;
        SearchResult sresult;

        EnvironmentOptions currentOptions;
        double heuristicWeight;
        int task[4];
        // task describes locations of start and finish:
        // 0 - start_i, 1 - start_j, 2 - goal_i, 3 - goal_j
        int maxSize;
        // maxSize is the maximum size of the considered grid
        bool isDijk;

    protected:
        int encode(int x, int y, int maxValue);

        void setHeuristic(Node& nodeToEdit);

        void expandNode(Node* nodeToExpand, const Map &map);

        void expandNodeDirection(Node* nodeToExpand, const Map &map, int i, int j);
};
#endif
