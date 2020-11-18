#include "search.h"
#include <chrono> 
//#define FIRST_MEET_EXIT
#define ENC(x, y) encode(x, y, maxSize)

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
    //!!!! options expected to be false everywhere
    //++++ check if the target mathes the start node

    // Initialise search parametres.
    int maxSize = std::max(map.getMapHeight(), map.getMapHeight());
    int task[4];
    map.getTask(task);

    // Create node to start.
    open[ENC(task[0], task[1])] = { task[0], task[1], 0 };
    setHeuristic(open[ENC(task[0], task[1])]);
    openHeap.insert(open[ENC(task[0], task[1])]);

    // Create node to search.
    open[ENC(task[2], task[3])] = { task[2], task[3], 0, 0 };
    targetNode = &open[ENC(task[2], task[3])];

    // Start the counter.
    auto start = std::chrono::high_resolution_clock::now();

    // Search in loop.
    sresult = SearchResult();
    while (open.size())
    {
        sresult.numberofsteps++;

        // Remove expanded node from the heap.
        Node* nodeToExpand = openHeap.popMin();

#ifdef FIRST_MEET_EXIT
        if (nodeToExpand == targetNode) {
            break;
        }
#endif

        // Expand the node.
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                // Don't use diagonal for now.
                if (i && j)
                {
                    continue;
                }

                // Check if considered node exists and is traversable.
                if (map.CellOnGrid(nodeToExpand->i + i, nodeToExpand->j + j) && map.CellIsTraversable(nodeToExpand->i + i, nodeToExpand->j + j))
                {
                    if (open.find(ENC(nodeToExpand->i + i, nodeToExpand->j + j)) != open.end() && 
                        open[ENC(nodeToExpand->i + i, nodeToExpand->j + j)].g > nodeToExpand->g + ch)
                    {
                        openHeap.decreaseGValue(open[ENC(nodeToExpand->i + i, nodeToExpand->j + j)], nodeToExpand->g + ch);

                        // Change parential node to the one, which is expanded.
                        open[ENC(nodeToExpand->i + i, nodeToExpand->j + j)].parent = &open[ENC(nodeToExpand->i, nodeToExpand->j)];
                    }
                    else if (close.find(ENC(nodeToExpand->i + i, nodeToExpand->j + j)) == close.end())
                    {
                        // Create new node.
                        open[ENC(nodeToExpand->i + i, nodeToExpand->j + j)] = Node{ nodeToExpand->i + i, nodeToExpand->j + j, nodeToExpand->g + ch };
                        setHeuristic(open[ENC(nodeToExpand->i + i, nodeToExpand->j + j)]);
                        openHeap.insert(open[ENC(nodeToExpand->i + i, nodeToExpand->j + j)]);

                        // Set parential node.
                        open[ENC(nodeToExpand->i + i, nodeToExpand->j + j)].parent = &open[ENC(nodeToExpand->i, nodeToExpand->j)];
                    }

                    // If potential new node is in close list, we never reopen it.
                }
            }
        }

        // Remove expanded node from maps.
        close[ENC(nodeToExpand->i, nodeToExpand->j)] = std::move(open[ENC(nodeToExpand->i, nodeToExpand->j)]);
        open.erase(ENC(nodeToExpand->i, nodeToExpand->j));
    }

    // Back propagation
    if (!targetNode->parent)
    {
        sresult.pathfound = true;
        sresult.hppath = &hppath;
        sresult.lppath = &lppath;
        sresult.pathlength = targetNode->g;

        // to-do
    }

    // Count result data.
    sresult.nodescreated = close.size() + open.size();

    std::chrono::duration<double> duration = start - std::chrono::high_resolution_clock::now();
    sresult.time = duration.count();

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
