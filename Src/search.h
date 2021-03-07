#pragma once

#include "nodes.h"
#include "map.h"
#include "searchresult.h"
#include "environmentoptions.h"
#include <vector>
#include <math.h>
#include <limits>
#include <chrono>
#include <unordered_map>
#include "config.h"
#include "agents.h"
#include "nodes_heap.h"
#include "gl_const.h"

// ----------------------- //
// SingleSearch definition //
// ----------------------- //

template<typename CellType>
class SingleSearch
{
public:
    using NodeType = Node<CellType>;
    using ResultType = SearchResult<CellType>;

protected:
    NodesBinaryHeap<CellType> open_;
    std::unordered_map<CellType, NodeType> nodes_;
    std::vector<NodeType> lppath_, hppath_;
    ResultType result_;

    EnvironmentOptions options_;
    AgentTask<CellType> task_;
    const Map* map_ = nullptr;
    double heuristic_weight_ = 0;
 
protected:
    virtual void SetHeuristic(NodeType& node_to_edit) = 0;

    virtual void ExpandNode(NodeType* node_to_expand) = 0;

    virtual void ExpandNodeMove(NodeType* node_to_expand, CellType destination);
    
    virtual const NodeType* CellIsReached(CellType cell) const;

    virtual bool NodeReachedCell(NodeType* node, CellType cell) const;

    virtual const Node<CellType>* ReachCell(CellType cell, FTYPE depth = -1);

    // Return -1 if the move isn't possible
    // Otherwise returns the cost of the move
    virtual FTYPE GetMoveCost(CellType from, CellType to) const = 0;

public:
    void SetConfiguration(const Map* map, const Config& config);

    const Node<CellType>* Plan(AgentTask<CellType> task, FTYPE depth = -1);
    
    FTYPE GetGValue(CellType cell) const;

    bool ContinueSearch(CellType cell);

    bool BuildPathTo(CellType cell);

    virtual void BuildCompactPath() = 0;

    ResultType GetResult() const;
};

// --------------------------- //
// SingleSearch implementation //
// --------------------------- //

template<typename CellType>
void SingleSearch<CellType>::SetConfiguration(const Map* map, const Config& config)
{
    // Set map
    map_ = map;

    // Initialise evironment parameters
    options_ = EnvironmentOptions(
        config.SearchParams[CN_SP_AS],
        config.SearchParams[CN_SP_AD],
        config.SearchParams[CN_SP_CC],
        config.SearchParams[CN_SP_MT]
    );

    // Set algorithm configurations
    if (config.SearchParams[CN_SP_ST] == CN_SP_ST_DIJK)
    {
        heuristic_weight_ = 0;
        open_.SetBreakTie(true);
    }
    else
    {
        heuristic_weight_ = config.SearchParams[CN_SP_HW];
        open_.SetBreakTie(config.SearchParams[CN_SP_BT] == CN_SP_BT_GMAX);
    }
}

template<typename CellType>
const Node<CellType>* SingleSearch<CellType>::Plan(AgentTask<CellType> task, FTYPE depth)
{
    // Clear previous planning
    open_.Clear();
    lppath_.clear();
    hppath_.clear();
    nodes_.clear();
    result_ = ResultType();

    // Set task
    task_ = task;

    // Init start node
    nodes_[task_.start] = NodeType(task_.start);
    NodeType& start_node = nodes_[task_.start];
    SetHeuristic(start_node);
    open_.Insert(start_node);

    // Plan a path
    return ReachCell(task_.goal, depth);
}

template<typename CellType>
const Node<CellType>* SingleSearch<CellType>::ReachCell(CellType cell, FTYPE depth)
{
    const Node<CellType>* reach_check = CellIsReached(cell);
    if (reach_check) return reach_check;

    // Set timer
    auto start = std::chrono::high_resolution_clock::now();

    // Search loop
    result_.pathfound = false;
    NodeType* expanded_node = nullptr;
    while (open_.Size() && !NodeReachedCell(expanded_node, cell))
    {
        result_.numberofsteps++;

        // Remove expanded node from the heap.
        expanded_node = open_.PopMin();

        // Expand the node.
        ExpandNode(expanded_node);

        // Mark expanded node as a node in the "close" list.
        expanded_node->h = -1;

        if (expanded_node->g >= depth && depth > 0) break; // for Plan function only
    }

    // Count statistics
    result_.pathlength = 0;
    result_.nodescreated = nodes_.size();

    // If the cell is reached expanded_node holds a suitable node
    if (NodeReachedCell(expanded_node, cell) || depth > 0)
    {
        result_.pathfound = true;
        result_.pathlength = expanded_node->g;
    }

    std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;
    result_.time += duration.count(); // in seconds

    if (!result_.pathfound)
    {
        return nullptr;
    }
    else
    {
        return expanded_node;
    }
}

template<typename CellType>
bool SingleSearch<CellType>::BuildPathTo(CellType cell)
{
    const NodeType* target_node = CellIsReached(cell);
    if (!target_node) return false;

    lppath_.clear();
    hppath_.clear();

    // Set timer
    auto start = std::chrono::high_resolution_clock::now();

    // Build path
    const NodeType* current_node = target_node;
    while (current_node)
    {
        // Copy
        lppath_.push_back(*current_node);
        // lppath_ will contain nodes with unsafe pointers (parent pointer)
        current_node = current_node->parent;

        lppath_.back().parent = nullptr;
    }

    std::reverse(lppath_.begin(), lppath_.end());

    result_.hppath = &hppath_;
    result_.lppath = &lppath_;

    // Count statistics
    std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;
    result_.time += duration.count(); // in seconds

    return true;
}

template<typename CellType>
FTYPE SingleSearch<CellType>::GetGValue(CellType cell) const
{
    const NodeType* node_reached_cell = CellIsReached(cell);
    if (node_reached_cell) return node_reached_cell->g;

    return -1;
}

template<typename CellType>
bool SingleSearch<CellType>::ContinueSearch(CellType cell)
{
    if (CellIsReached(cell)) return true;

    ReachCell(cell);

    if (CellIsReached(cell)) return true;
    return false;
}

template<typename CellType>
void SingleSearch<CellType>::ExpandNodeMove(NodeType* node_to_expand, CellType destination)
{
    // Check if the move is possible
    FTYPE cost = GetMoveCost(node_to_expand->cell, destination);
    if (cost == -1) return;

    // Check if a potential node exists
    auto potential_node = nodes_.find(destination);
    if (potential_node == nodes_.end())
    {
        // Create a new node.
        auto insert_result = nodes_.insert({ destination, { destination, node_to_expand->g + cost } });
        NodeType& inserted_node = insert_result.first->second;

        // Insert in the heap
        SetHeuristic(inserted_node);
        open_.Insert(inserted_node);

        // Set the parential node.
        inserted_node.parent = node_to_expand;
    }
    else
    {
        if (potential_node->second.h >= 0 && potential_node->second.g > node_to_expand->g + cost)
        {
            open_.DecreaseGValue(potential_node->second, node_to_expand->g + cost);

            // Change the parential node to the one which is expanded.
            potential_node->second.parent = node_to_expand;
        }
        // If the potential node is in the close list, we never reopen/reexpand it.
    }
}

template<typename CellType>
const Node<CellType>* SingleSearch<CellType>::CellIsReached(CellType cell) const
{
    auto potential_node = nodes_.find(cell);

    if (potential_node == nodes_.end()) return nullptr;
    return &potential_node->second;
}

template<typename CellType>
bool SingleSearch<CellType>::NodeReachedCell(NodeType* node, CellType cell) const
{
    if (nullptr == node) return false;
    return node->cell == cell;
}

template<typename CellType>
SearchResult<CellType> SingleSearch<CellType>::GetResult() const
{
    return result_;
}
