#pragma once

#include <cassert>
#include "agents.h"
#include "agents_array.h"
#include "mapf_interface.h"
#include "astar.h"
#include "astar3d.h"
#include <vector>
#include <unordered_map>

// MAPF solver with limited depth of search
class WHCA : public MAPFSolverInterface<SpaceTimeCell>
{
protected:
    std::unordered_set<SpaceTimeCell> reservation_;
    std::unordered_set<SpaceTimeCell> diagonals_;

    GridSingleSearch space_solver_;
    SpaceTimeSearch space_time_solver_;

    std::unordered_map<int, AgentTask<SpaceTimeCell>> tasks_;
    std::unordered_map<int, SearchResult<SpaceTimeCell>> results_;
    std::unordered_map<int, std::vector<Node<SpaceTimeCell>>> paths_;
    AgentsArray<SpaceTimeCell> agents_;
    
    const Map* map_ = nullptr;
    const Config* config_ = nullptr;

    // If true before each planning agents will be reordered randomly
    unsigned int random_order : 1;
    // Depth of cooperation
    int depth_;
    // Counter to move time and change the locations of agents
    TTYPE extra_time_;
    // The number of agents on their goals in 2d in the last time unit
    int on_goal_;
    
protected:
    static int number_of_swaps;
    static void RandomReorder(std::vector<int>& vec);

public:
    WHCA();

    void SetDepth(FTYPE new_depth);

    virtual void ResetConfiguration(const Map* map, const Config& config) override;

    virtual int AddAgent(AgentTask<SpaceTimeCell> task) override;

    virtual void RemoveAgent(int agent_ID) override;

    virtual bool Plan() override;

    SearchResult<SpaceTimeCell> GetPlan(int agent_ID) const;

    void MoveTime(TTYPE delta_time);

    int NumOfGoalsReached() const;

    int GetDepth() const;

    const std::unordered_set<SpaceTimeCell>* GetResTable() const;
};
