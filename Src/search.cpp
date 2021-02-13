#pragma once

#include "search.h"
#include <chrono>
#include <iostream>
#include <algorithm>
#define ENC(x, y) encode(x, y, maxSize)
#define EXPEXTED_OPEN_NODES 32

const double ch = 1;
const double cd = std::sqrt(2);

NodesBinaryHeap::NodesBinaryHeap()
{
    nodes = { nullptr };
    isBreakTieGMax = true;
    nodes.reserve(EXPEXTED_OPEN_NODES);
}

void NodesBinaryHeap::moveUp(size_t nodeIndex)
{
    for (size_t parentIndex = (nodeIndex >> 1); parentIndex && compare(*nodes[parentIndex], *nodes[nodeIndex]); nodeIndex >>= 1, parentIndex >>= 1)
    {
        std::swap(nodes[parentIndex]->heapIndex, nodes[nodeIndex]->heapIndex);
        std::swap(nodes[parentIndex], nodes[nodeIndex]);
    }
}

void NodesBinaryHeap::moveDown(size_t nodeIndex)
{
    for (size_t minChildIndex = nodeIndex << 1; minChildIndex + 1 < nodes.size(); minChildIndex = nodeIndex << 1)
    {
        if (compare(*nodes[minChildIndex], *nodes[minChildIndex + 1]))
            ++minChildIndex;

        Node& currentNode = *nodes[nodeIndex];
        Node& minChild = *nodes[minChildIndex];
        if (!compare(currentNode, minChild))
            break;

        std::swap(currentNode.heapIndex, minChild.heapIndex);
        std::swap(nodes[nodeIndex], nodes[minChildIndex]);
        nodeIndex = minChildIndex;
    }

    if ((nodeIndex << 1) < nodes.size())
    {
        Node& currentNode = *nodes[nodeIndex];
        Node& minChild = *nodes[nodeIndex << 1];

        if (compare(currentNode, minChild))
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
    if (nodes.size() <= 1)
    {
        return nullptr;
    }

    Node* result = nodes[1];
    std::swap(nodes[1], nodes[nodes.size() - 1]);
    nodes.pop_back();
    if (nodes.size() >= 2)
    {
        nodes[1]->heapIndex = 1;
    }
    moveDown(1);

    return result;
}

size_t NodesBinaryHeap::size()
{
    return nodes.size() ? nodes.size() - 1 : 0;
}

void NodesBinaryHeap::setBreakTie(bool breakTieGMax)
{
    isBreakTieGMax = breakTieGMax;
}

bool NodesBinaryHeap::compare(const Node& first, const Node& second)
{
    double fFirst = first.g + first.H;
    double fSecond = second.g + second.H;
    
    if (fFirst == fSecond)
    {
        return isBreakTieGMax == (first.g < second.g);
    }

    return fFirst > fSecond;
}

SingleSearch::SingleSearch()
{
    map_ = nullptr;
    heuristicWeight = 0;
    isDijk = false;
    maxSize = 0;
    task_ = { 0, 0, 0, 0 };
}

SingleSearch::~SingleSearch() {}

void SingleSearch::SetConfiguration(const Map* map, const EnvironmentOptions &options, const Config& config)
{
    // Set map
    map_ = map;

    // Initialise search parametres.
    currentOptions = options;
    maxSize = std::max(map->GetHeight(), map->GetHeight());

    // Set algorithm configurations.
    isDijk = (config.SearchParams[CN_SP_ST] == CN_SP_ST_DIJK);

    if (isDijk)
    {
        heuristicWeight = 0;
        openHeap.setBreakTie(true);
    }
    else
    {
        heuristicWeight = config.SearchParams[CN_SP_HW];
        openHeap.setBreakTie(config.SearchParams[CN_SP_BT] == CN_SP_BT_GMAX);
    }
}

int SingleSearch::AddAgent(AgentTask task)
{
    // Get task
    task_ = task;

    return 0;
}

void SingleSearch::RemoveAgent(AgentIDType agent_ID)
{
    return;
}

void SingleSearch::Plan(bool full_plan)
{
    // Start the counter.
    auto start = std::chrono::high_resolution_clock::now();

    sresult = SearchResult();
    if (!map_->IsCellTraversable(task_.start_i, task_.start_j))
    {
        // Default constructor of SearchResult will already have correct parameters.
        return;
    }

    // Create node to start.
    generatedNodes[ENC(task_.start_i, task_.start_j)] = { task_.start_i, task_.start_j, 0 };
    Node* startNode = &generatedNodes[ENC(task_.start_i, task_.start_j)];
    setHeuristic(*startNode);
    openHeap.insert(*startNode);

    // Search in loop.
    while (openHeap.size())
    {
        sresult.numberofsteps++;

        // Remove expanded node from the heap.
        Node* nodeToExpand = openHeap.popMin();

        if (nodeToExpand->i == task_.goal_i && nodeToExpand->j == task_.goal_j) {
            break;
        }

        // Expand the node.
        expandNode(nodeToExpand, map_);

        // Mark expanded node as a node in the "close" list.
        nodeToExpand->H = -1;
    }

    // Back propagation
    if (generatedNodes.find(ENC(task_.goal_i, task_.goal_j)) != generatedNodes.end())
    {
        Node* targetNode = &generatedNodes[ENC(task_.goal_i, task_.goal_j)];

        sresult.pathfound = true;
        sresult.pathlength = targetNode->g;

        Node* currentNode = targetNode;
        while (currentNode)
        {
            // For now, nodes are copied to lppath, not moved.
            lppath.push_front(*currentNode);
            currentNode = currentNode->parent;
        }

        sresult.hppath = &hppath; // For now, hppath isn't created
        sresult.lppath = &lppath;
    }

    // Count result data.
    sresult.nodescreated = generatedNodes.size();

    std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;
    sresult.time = duration.count(); // in seconds

    if (sresult.pathfound)
    {
        // Create hppath
        int previousDirection[2] = { 0, 0 };
        int currentDirection[2] = { 0, 0 };

        auto pathIterator = lppath.begin();
        hppath.push_back(*pathIterator);
        Node lastNode = *(pathIterator++);

        while (lppath.end() != pathIterator)
        {
            currentDirection[0] = pathIterator->i - lastNode.i;
            currentDirection[1] = pathIterator->j - lastNode.j;

            if (currentDirection[0] == previousDirection[0] && currentDirection[1] == previousDirection[1])
            {
                hppath.back() = *pathIterator;
            }
            else
            {
                hppath.emplace_back(*pathIterator);
            }

            previousDirection[0] = currentDirection[0];
            previousDirection[1] = currentDirection[1];
            lastNode = *pathIterator;
            pathIterator++;
        }
    }
}

SearchResult SingleSearch::GetPlan(AgentIDType agent_ID) const
{
    return sresult;
}

void SingleSearch::setHeuristic(Node& nodeToEdit)
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

    double dx = abs(nodeToEdit.i - task_.goal_i);
    double dy = abs(nodeToEdit.j - task_.goal_j);

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

int SingleSearch::encode(int x, int y, int maxValue)
{
    // Hash table requires a hashable type, so std::pair won't suit.
    // This encoding provides one-to-one correspondence between two coordinates and int.
    return x + y * maxValue;
}

void SingleSearch::expandNodeDirection(Node* nodeToExpand, const Map *map, int i, int j)
{
    // Check possibility of movement
    if (!map->IsCellOnGrid(nodeToExpand->i + i, nodeToExpand->j + j) ||
        !map->IsCellTraversable(nodeToExpand->i + i, nodeToExpand->j + j))
    {
        return;
    }

    double cValue = ch;

    // Check if the current step goes through the diagonal.
    if (i && j)
    {
        cValue = cd;
        if (!currentOptions.allowdiagonal)
        {
            return;
        }

        // Count untraversable cells on the diagonal path.
        char count = !map->IsCellTraversable(nodeToExpand->i, nodeToExpand->j + j) + \
            !map->IsCellTraversable(nodeToExpand->i + i, nodeToExpand->j);

        // Check if the path through the diagonal is possible.
        if ((count > 0 && !currentOptions.cutcorners) || (count == 2 && !currentOptions.allowsqueeze))
        {
            return;
        }
    }

    // Check if the considered node exists and is traversable.
    int nodeKey = ENC(nodeToExpand->i + i, nodeToExpand->j + j);
    auto potentialNode = generatedNodes.find(nodeKey);
    if (potentialNode == generatedNodes.end())
    {
        // Create a new node.
        auto insertResult = generatedNodes.insert({ nodeKey, { nodeToExpand->i + i, nodeToExpand->j + j, nodeToExpand->g + cValue } });
        Node& insertedNode = insertResult.first->second;

        // Insert in the heap
        setHeuristic(insertedNode);
        openHeap.insert(insertedNode);

        // Set the parential node.
        insertedNode.parent = nodeToExpand;
    }
    else
    {
        if (potentialNode->second.H >= 0 && potentialNode->second.g > nodeToExpand->g + cValue)
        {
            openHeap.decreaseGValue(potentialNode->second, nodeToExpand->g + cValue);

            // Change the parential node to the one which is expanded.
            potentialNode->second.parent = nodeToExpand;
        }
        // If the potential node is in the close list, we never reopen/reexpand it.
    }
}

void SingleSearch::expandNode(Node* nodeToExpand, const Map *map)
{
    expandNodeDirection(nodeToExpand, map, -1, -1);
    expandNodeDirection(nodeToExpand, map, -1, 0);
    expandNodeDirection(nodeToExpand, map, -1, 1);
    expandNodeDirection(nodeToExpand, map, 0, -1);
    expandNodeDirection(nodeToExpand, map, 0, 1);
    expandNodeDirection(nodeToExpand, map, 1, -1);
    expandNodeDirection(nodeToExpand, map, 1, 0);
    expandNodeDirection(nodeToExpand, map, 1, 1);
}

#undef ENC
#undef EXPEXTED_OPEN_NODES