#pragma once
#include "search.h"
#include <chrono>
#include <algorithm>
#define FIRST_MEET_EXIT
#define ENC(x, y) encode(x, y, maxSize)

const double ch = 1;
const double cd = std::sqrt(2);

NodesBinaryHeap::NodesBinaryHeap()
{
    nodes = { nullptr };
}

void NodesBinaryHeap::moveUp(size_t nodeIndex)
{
    for (size_t parentIndex = nodeIndex >> 1; parentIndex && *nodes[parentIndex] > *nodes[nodeIndex]; nodeIndex >>= 1, parentIndex >>= 1)
    {
        std::swap(nodes[parentIndex]->heapIndex, nodes[nodeIndex]->heapIndex);
        std::swap(nodes[parentIndex], nodes[nodeIndex]);
    }
}

void NodesBinaryHeap::moveDown(size_t nodeIndex)
{
    for (size_t minChildIndex = nodeIndex << 1; minChildIndex + 1 < nodes.size(); minChildIndex = nodeIndex << 1)
    {
        if (*nodes[minChildIndex] > *nodes[minChildIndex + 1])
            ++minChildIndex;

        Node& currentNode = *nodes[nodeIndex];
        Node& minChild = *nodes[minChildIndex];
        if (!(currentNode > minChild))
            break;

        std::swap(currentNode.heapIndex, minChild.heapIndex);
        std::swap(nodes[nodeIndex], nodes[minChildIndex]);
        nodeIndex = minChildIndex;
    }
    if ((nodeIndex << 1) < nodes.size())
    {
        Node& currentNode = *nodes[nodeIndex];
        Node& minChild = *nodes[nodeIndex << 1];

        if (currentNode > minChild)
        {
            std::swap(currentNode.heapIndex, minChild.heapIndex);
            std::swap(nodes[nodeIndex], nodes[nodeIndex << 1]);
        }
    }
}

void NodesBinaryHeap::insert(Node& newNode)
{
    newNode.heapIndex = nodes.size();
    nodes.emplace_back(&newNode);
    moveUp(newNode.heapIndex);
}

void NodesBinaryHeap::decreaseGValue(Node& nodeToChange, double newGValue)
{
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

unsigned int NodesBinaryHeap::size()
{
    return nodes.size() - 1;
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
        // Default constructor of SearchResult will already have correct parameters.
        return sresult;
    }

    // Create node to start.
    generatedNodes[ENC(task[0], task[1])] = { task[0], task[1], 0 };
    Node* startNode = &generatedNodes[ENC(task[0], task[1])];
    setHeuristic(*startNode);
    openHeap.insert(*startNode);

    // Search in loop.
    while (openHeap.size())
    {
        sresult.numberofsteps++;

        // Remove expanded node from the heap.
        Node* nodeToExpand = openHeap.popMin();

#ifdef FIRST_MEET_EXIT
        if (nodeToExpand->i == task[2] && nodeToExpand->j == task[3]) {
            break;
        }
#endif

        // Expand the node.
        expandNode(nodeToExpand, map);

        // Remove expanded node from maps.
        nodeToExpand->isInClose = true;
    }

    // Back propagation
    if (generatedNodes.find(ENC(task[2], task[3])) != generatedNodes.end())
    {
        Node* targetNode = &generatedNodes[ENC(task[2], task[3])];

        sresult.pathfound = true;
        sresult.pathlength = targetNode->g;

        Node* currentNode = targetNode;
        while (currentNode)
        {
            // For now, nodes are copied to lppath, not moved.
            lppath.push_front(*currentNode);
            currentNode = currentNode->parent;
        }

        sresult.hppath = &lppath; // For now, hppath isn't created
        sresult.lppath = &lppath;
    }

    // Count result data.
    sresult.nodescreated = generatedNodes.size();

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

int Search::encode(int x, int y, int maxValue)
{
    // Hash table requires a hashable type, so std::pair won't suit.
    // This encoding provides one-to-one correspondence between two coordinates and int.
    return x + y * maxValue;
}

void Search::expandNode(Node* nodeToExpand, const Map &map)
{
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            // Check possibility of movement
            if (!(i || j) || 
                !map.CellOnGrid(nodeToExpand->i + i, nodeToExpand->j + j) || 
                !map.CellIsTraversable(nodeToExpand->i + i, nodeToExpand->j + j))
            {
                continue;
            }

            double cValue = ch;

            // Check if the current step goes through the diagonal.
            if (i && j)
            {
                cValue = cd;
                if (!currentOptions.allowdiagonal)
                {
                    continue;
                }

                // Count untraversable cells on the diagonal path.
                char count = !map.CellIsTraversable(nodeToExpand->i, nodeToExpand->j + j) + \
                    !map.CellIsTraversable(nodeToExpand->i + i, nodeToExpand->j);

                // Check if the path through the diagonal is possible.
                if ( (count > 0 && !currentOptions.cutcorners) || (count == 2 && !currentOptions.allowsqueeze) )
                {
                    continue;
                }
            }

            // Check if the considered node exists and is traversable.
            int nodeKey = ENC(nodeToExpand->i + i, nodeToExpand->j + j);
            auto potentialNode = generatedNodes.find(nodeKey);
            if (potentialNode == generatedNodes.end())
            {
                // Create a new node.
                auto insertResult = generatedNodes.insert({nodeKey, { nodeToExpand->i + i, nodeToExpand->j + j, nodeToExpand->g + cValue }});
                Node& insertedNode = insertResult.first->second;
                //++++ check if insertion fails
                setHeuristic(insertedNode);
                openHeap.insert(insertedNode);

                // Set the parential node.
                insertedNode.parent = nodeToExpand;
            }
            else
            {
                if (!potentialNode->second.isInClose && potentialNode->second.g > nodeToExpand->g + cValue)
                {
                    openHeap.decreaseGValue(potentialNode->second, nodeToExpand->g + cValue);

                    // Change the parential node to the one which is expanded.
                    potentialNode->second.parent = nodeToExpand;
                }

                // If the potential node is in the close list, we never reopen/reexpand it.
            }
        }
    }
}

#undef ENC(x, y)
#undef FIRST_MEET_EXIT
