#include "search.h"

const double ch = 1;
const double cd = std::sqrt(2);

NodesBinaryHeap::NodesBinaryHeap()
{
    nodes = { nullptr };
}

void NodesBinaryHeap::moveUp(size_t nodeIndex)
{
    if (nodeIndex >= nodes.size())
    {
        // throw something
        return;
    }

    while (nodeIndex > 1 && *nodes[nodeIndex / 2] > *nodes[nodeIndex])
    {
        std::swap(nodes[nodeIndex], nodes[nodeIndex / 2]);
        std::swap(nodes[nodeIndex]->heapIndex, nodes[nodeIndex / 2]->heapIndex);
        nodeIndex /= 2;
    }
}

void NodesBinaryHeap::moveDown(size_t nodeIndex)
{
    if (nodeIndex >= nodes.size())
    {
        // throw something
        return;
    }

    size_t minNodeIndex = nodeIndex;
    do
    {
        nodeIndex = minNodeIndex;

        if (nodeIndex * 2 + 1 < nodes.size() && *nodes[minNodeIndex] > *nodes[nodeIndex * 2 + 1])
        {
            minNodeIndex = nodeIndex * 2 + 1;
        }

        if (nodeIndex * 2 < nodes.size() && *nodes[minNodeIndex] > *nodes[nodeIndex * 2])
        {
            minNodeIndex = nodeIndex * 2;
        }

        if (minNodeIndex != nodeIndex)
        {
            std::swap(nodes[nodeIndex], nodes[minNodeIndex]);
            std::swap(nodes[nodeIndex]->heapIndex, nodes[minNodeIndex]->heapIndex);
        }
    } while (minNodeIndex != nodeIndex);
}

void NodesBinaryHeap::insert(Node& newNode)
{
    newNode.heapIndex = nodes.size();
    nodes.push_back(&newNode);
    moveUp(newNode.heapIndex);
}

void NodesBinaryHeap::decreaseGValue(Node& nodeToChange, double newGValue)
{
    if (newGValue >= nodeToChange.g)
    {
        // throw something
        return;
    }

    nodeToChange.g = newGValue;
    moveUp(nodeToChange.heapIndex);
}

Node* NodesBinaryHeap::popMin()
{
    if (nodes.size() == 1)
    {
        return nullptr;
    }

    Node* result = nodes[1];
    std::swap(nodes[1], nodes[nodes.size() - 1]);
    nodes.pop_back();
    moveDown(1);

    return result;
}


Search::Search()
{
    //set defaults here
}

Search::~Search() {}

SearchResult Search::startSearch(ILogger *Logger, const Map &map, const EnvironmentOptions &options)
{
    // options expected to be false everywhere

    // Initialise search parametres.
    int maxSize = std::max(map.getMapHeight(), map.getMapHeight());
    int task[4];
    map.getTask(task);

    // Create node to start and node to search.
    open[encode(task[0], task[1], maxSize)] = { task[0], task[1], 0 };
    setHeuristic(open[encode(task[0], task[1], maxSize)]);

    open[encode(task[1], task[2], maxSize)] = { task[2], task[3], 0, 0 };
    targetNode = &open[encode(task[1], task[2], maxSize)];

    //need to implement
    SearchResult sresult;
    /*sresult.pathfound = ;
    sresult.nodescreated =  ;
    sresult.numberofsteps = ;
    sresult.time = ;
    sresult.hppath = &hppath; //Here is a constant pointer
    sresult.lppath = &lppath;*/
    return sresult;
}

void Search::setHeuristic(Node& nodeToEdit)
{
    if (nodeToEdit.H >= 0)
    {
        return;
    }

    nodeToEdit.H = 0;
}

int Search::encode(int x, int y, int maxValue)
{
    return x + y * maxValue;
}

void Search::expandNode(Node* node)
{

}

/*void Search::makePrimaryPath(Node curNode)
{
    //need to implement
}*/

/*void Search::makeSecondaryPath()
{
    //need to implement
}*/
