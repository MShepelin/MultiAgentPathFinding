#pragma once
#include "search.h"
#include <chrono>
#include <algorithm>
#define ENCODE_GRID_POSITION(x, y) (x) + (y) * (long long) maxSize

const double ch = 1;
const double cd = std::sqrt(2);

NodesBinaryHeap::NodesBinaryHeap()
{
    nodes = { Node() };
}

void NodesBinaryHeap::moveUp(size_t nodeIndex)
{
    for (; nodeIndex > 1 && nodes[nodeIndex >> 1] > nodes[nodeIndex]; nodeIndex >>= 1)
    {
        std::swap(keyToIndex[nodes[nodeIndex].key], keyToIndex[nodes[nodeIndex >> 1].key]);
        std::swap(nodes[nodeIndex], nodes[nodeIndex >> 1]);
    }
}

void NodesBinaryHeap::moveDown(size_t nodeIndex)
{
    for (size_t minChildIndex = nodeIndex << 1; minChildIndex < nodes.size(); minChildIndex = nodeIndex << 1)
    {
        if (minChildIndex + 1 < nodes.size() && nodes[minChildIndex] > nodes[minChildIndex + 1])
            ++minChildIndex;

        Node& currentNode = nodes[nodeIndex];
        Node& minChild = nodes[minChildIndex];
        if (!(currentNode > minChild))
            break;

        std::swap(keyToIndex[nodes[nodeIndex].key], keyToIndex[nodes[nodeIndex >> 1].key]);
        std::swap(nodes[nodeIndex], nodes[minChildIndex]);
        nodeIndex = minChildIndex;
    }
}

void NodesBinaryHeap::insert(Node& newNode)
{
    // Prepare node to extract data from it.
    size_t nodeIndex = nodes.size();
    keyToIndex.insert({ newNode.key, nodeIndex });
    nodes.emplace_back(newNode);
    moveUp(nodeIndex);
}

void NodesBinaryHeap::decreaseGValue(long long key, double newGValue, Node* newParent)
{
    size_t nodeIndex = keyToIndex[key];

    nodes[nodeIndex].g = newGValue;
    nodes[nodeIndex].parent = newParent;
    moveUp(nodeIndex);
}

Node NodesBinaryHeap::popMin()
{
    /*
    if (nodes.size() == 1)
    {
        // error
        return Node();
    }*/

    Node result = nodes[1];
    std::swap(nodes[1], nodes[nodes.size() - 1]);
    keyToIndex[nodes[1].key] = 1;
    nodes.pop_back();
    moveDown(1);

    return result;
}

size_t NodesBinaryHeap::size()
{
    return nodes.size() - 1;
}

Node* NodesBinaryHeap::getNode(long long key)
{
    auto iterator = keyToIndex.find(key);
    if (iterator == keyToIndex.end())
    {
        return nullptr;
    }
    return &nodes[keyToIndex[iterator->second]];
}

Search::Search()
{
    heuristicWeight = 0;
    isDijk = false;
    maxSize = 0;
    std::fill(task, task + 4, 0);
}

Search::~Search() {}

SearchResult Search::startSearch(ILogger *Logger, const Map &map, const EnvironmentOptions &options, const Config& config)
{
    // Initialise search parametres.
    currentOptions = options;
    maxSize = std::max(map.getMapHeight(), map.getMapHeight());
    map.getTask(task);

    // Set algorithm configurations.
    isDijk = (config.SearchParams[CN_SP_ST] == CN_SP_ST_DIJK);
    heuristicWeight = config.SearchParams[CN_SP_HW];

    // Start the counter.
    auto start = std::chrono::high_resolution_clock::now();

    sresult = SearchResult();
    if (!map.CellIsTraversable(task[0], task[1]))
    {
        // Default constructor of SearchResult will set correct parameters.
        return sresult;
    }

    // Create node to start.
    Node startNode = { task[0], task[1], 0 };
    startNode.key = ENCODE_GRID_POSITION(task[0], task[1]);
    setHeuristic(startNode);
    openHeap.insert(startNode);

    // Try to expand a node.
    Node nodeToExpand;
    while (openHeap.size())
    {
        nodeToExpand = openHeap.popMin();
        ++sresult.numberofsteps;

        if (nodeToExpand.i == task[2] && nodeToExpand.j == task[3]) {
            break;
        }

        // Add to the "close" list.
        close.insert({ ENCODE_GRID_POSITION(task[2], task[3]), nodeToExpand });

        // Expand the node.
        expandNode(close[ENCODE_GRID_POSITION(task[2], task[3])], map);
    }

    // Back propagation
    if (close.find(ENCODE_GRID_POSITION(task[2], task[3])) != close.end())
    {
        Node& targetNode = close[ENCODE_GRID_POSITION(task[2], task[3])];

        sresult.pathfound = true;
        sresult.pathlength = targetNode.g;

        Node* currentNode = &targetNode;
        while (currentNode)
        {
            // For now, nodes are copied to lppath, not moved.
            lppath.push_front(*currentNode);
            currentNode = currentNode->parent;
        }

        sresult.hppath = &lppath; // For now, hppath isn't created
        sresult.lppath = &lppath;
    }

    std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;
    sresult.time = duration.count(); // in seconds

    return sresult;
}

void Search::setHeuristic(Node& nodeToEdit)
{
    if (nodeToEdit.H >= 0)
    {
        // Not to change already calculated heuristic.
        return;
    }

    if (isDijk)
    {
        nodeToEdit.H = 0;
        return;
    }

    double dx = abs(nodeToEdit.i - task[2]);
    double dy = abs(nodeToEdit.j - task[3]);

    switch (currentOptions.metrictype)
    {
    case CN_SP_MT_DIAG:
        nodeToEdit.H = ch*abs(dx - dy) + cd*std::min(dx, dy);
        break;

    case CN_SP_MT_MANH:
        nodeToEdit.H = ch * (dx + dy);
        break;

    case CN_SP_MT_EUCL:
        nodeToEdit.H = ch * std::sqrt(dx*dx + dy * dy);
        break;

    case CN_SP_MT_CHEB:
        nodeToEdit.H = std::max(dx, dy);
        break;

    default:
        nodeToEdit.H = 0;
        break;
    }

    nodeToEdit.H *= heuristicWeight;
}

void Search::expandNode(Node& nodeToExpand, const Map &map)
{
    int nodeI = nodeToExpand.i;
    int nodeJ = nodeToExpand.j;
    double nodeGValue = nodeToExpand.g;

    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            // Check possibility of movement
            if (!(i || j) || 
                !map.CellOnGrid(nodeI + i, nodeJ + j) ||
                !map.CellIsTraversable(nodeI + i, nodeJ + j))
            {
                continue;
            }

            double cValue = ch;

            // Check if the current step goes through the diagonal.
            if (i && j)
            {
                if (!currentOptions.allowdiagonal)
                    continue;

                cValue = cd;

                // Count untraversable cells on the diagonal path.
                char count = !map.CellIsTraversable(nodeI, nodeJ + j) + \
                    !map.CellIsTraversable(nodeI + i, nodeJ);

                // Check if the path through the diagonal is possible.
                if ( (count && !currentOptions.cutcorners) || (count == 2 && !currentOptions.allowsqueeze) )
                    continue;
            }

            // Check if "close" or "open" list contains considered node.
            long long nodeKey = ENCODE_GRID_POSITION(nodeI + i, nodeJ + j);
            Node* potentialNode = openHeap.getNode(nodeKey);
            if (potentialNode)
            {
                if (potentialNode->g > nodeGValue + cValue)
                    openHeap.decreaseGValue(nodeKey, nodeGValue + cValue, &nodeToExpand);
            }
            else if (close.find(nodeKey) == close.end())
            {
                // Create a new node.
                ++sresult.nodescreated;
                Node newNode = { nodeI + i, nodeJ + j, nodeGValue + cValue };
                newNode.parent = &nodeToExpand;
                newNode.key = ENCODE_GRID_POSITION(nodeI + i, nodeJ + j);
                setHeuristic(newNode);
                openHeap.insert(newNode);
            }

            // If the potential node is in the close list, we never reopen/reexpand it.
        }
    }
}

#undef ENCODE_GRID_POSITION(x, y)
