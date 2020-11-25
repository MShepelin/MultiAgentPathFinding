#include "search.h"
#include <chrono>
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

unsigned int NodesBinaryHeap::size()
{
    return nodes.size() - 1;
}


Search::Search()
{
    //set defaults here
}

Search::~Search() {}

SearchResult Search::startSearch(ILogger *Logger, const Map &map, const EnvironmentOptions &options, const Config& config)
{
    // Initialise search parametres.
    currentOptions = options;
    maxSize = std::max(map.getMapHeight(), map.getMapHeight());
    map.getTask(task);

    // Set algorithm type.
    isDijk = (config.SearchParams[CN_SP_ST] == CN_SP_ST_DIJK);

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
    sresult.time = duration.count();

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
}

int Search::encode(int x, int y, int maxValue)
{
    return x + y * maxValue;
}

void Search::expandNode(Node* nodeToExpand, const Map &map)
{
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (!map.CellOnGrid(nodeToExpand->i + i, nodeToExpand->j + j) || !map.CellIsTraversable(nodeToExpand->i + i, nodeToExpand->j + j))
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
                char count = 0;
                if (!map.CellIsTraversable(nodeToExpand->i, nodeToExpand->j + j))
                {
                    count++;
                }

                if (!map.CellIsTraversable(nodeToExpand->i + i, nodeToExpand->j))
                {
                    count++;
                }

                // Check if the path through the diagonal is possible.
                if (count > 0 && !currentOptions.cutcorners)
                {
                    continue;
                }
                if (count == 2 && !currentOptions.allowsqueeze)
                {
                    continue;
                }
            }

            // Check if the considered node exists and is traversable.
            Node* potentialNode;
            if (generatedNodes.find(ENC(nodeToExpand->i + i, nodeToExpand->j + j)) == generatedNodes.end())
            {
                // Create a new node.
                generatedNodes[ENC(nodeToExpand->i + i, nodeToExpand->j + j)] = Node{ nodeToExpand->i + i, nodeToExpand->j + j, nodeToExpand->g + cValue };
                potentialNode = &generatedNodes[ENC(nodeToExpand->i + i, nodeToExpand->j + j)];
                setHeuristic(*potentialNode);
                openHeap.insert(*potentialNode);

                // Set the parential node.
                potentialNode->parent = nodeToExpand;
            }
            else
            {
                potentialNode = &generatedNodes[ENC(nodeToExpand->i + i, nodeToExpand->j + j)];
                if (!potentialNode->isInClose && potentialNode->g > nodeToExpand->g + cValue)
                {
                    openHeap.decreaseGValue(*potentialNode, nodeToExpand->g + cValue);

                    // Change the parential node to the one which is expanded.
                    potentialNode->parent = nodeToExpand;
                }

                // If the potential node is in the close list, we never reopen/reexpand it.
            }
        }
    }
}
