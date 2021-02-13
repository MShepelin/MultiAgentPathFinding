#pragma once

#include "map.h"
#include "mapf_interface.h"
#include "searchresult.h"
#include "environmentoptions.h"
#include <list>
#include <vector>
#include <math.h>
#include <limits>
#include <chrono>
#include <unordered_map>
#include "config.h"
#include "agents.h"

class NodesBinaryHeap
{
private:
    std::vector<Node*> nodes;
    bool isBreakTieGMax;

    void moveUp(size_t nodeIndex);
    void moveDown(size_t nodeIndex);

public:
    NodesBinaryHeap();

    // Returns true if the first node has greater order than the second one
    bool compare(const Node& first, const Node& second);

    Node* popMin();

    void insert(Node& newNode);

    void decreaseGValue(Node& nodeToChange, double newGValue);

    size_t size();

    void setBreakTie(bool breakTieGMax);
};

class SingleSearch : public MAPFSolverInterface
{
public:
    SingleSearch();
    ~SingleSearch(void);

    virtual void SetConfiguration(const Map* map, const EnvironmentOptions &options, const Config& config) override;

    virtual AgentIDType AddAgent(AgentTask task) override;

    virtual void RemoveAgent(AgentIDType agent_ID) override;

    virtual void Plan(bool full_plan = true) override;

    virtual SearchResult GetPlan(AgentIDType agent_ID) const override;

protected:
    NodesBinaryHeap openHeap;
    std::unordered_map<int, Node> generatedNodes;
    std::list<Node> lppath, hppath;
    SearchResult sresult;

    EnvironmentOptions currentOptions;
    double heuristicWeight;
    AgentTask task_;
    int maxSize;
    // maxSize is the maximum size of the considered grid
    bool isDijk;
    const Map* map_;

protected:
    static int encode(int x, int y, int maxValue);

    void setHeuristic(Node& nodeToEdit);

    void expandNode(Node* nodeToExpand, const Map *map);

    void expandNodeDirection(Node* nodeToExpand, const Map *map, int i, int j);
};
