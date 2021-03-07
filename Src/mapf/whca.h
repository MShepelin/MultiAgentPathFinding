#pragma once

#include <cassert>
#include "agents.h"
#include "agents_array.h"
#include "mapf_interface.h"
#include "astar.h"
#include "astar3d.h"
#include <vector>
#include <stack>
#include <unordered_map>

class WHCA : public MAPFSolverInterface<SpaceTimeCell>
{
protected:
    std::unordered_set<SpaceTimeCell> reservation_;

    GridSingleSearch space_solver_;
    SpaceTimeSearch space_time_solver_;

    std::unordered_map<int, AgentTask<SpaceTimeCell>> tasks_;
    std::unordered_map<int, std::stack<SpaceTimeCell>> paths_;
    AgentsArray<SpaceTimeCell> agents_;
    
    const Map* map_ = nullptr;
    const Config* config_ = nullptr;

    FTYPE depth_ = -1;

    int extra_time_ = 0;

    // The number of agents on their goals in 2d in the last time unit
    int on_goal_ = 0; 
    
public:
    WHCA();

    void SetDepth(FTYPE new_depth);

    virtual void SetConfiguration(const Map* map, const Config& config) override;

    virtual int AddAgent(AgentTask<SpaceTimeCell> task) override;

    virtual void RemoveAgent(int agent_ID) override;

    virtual bool Plan() override;

    const std::stack<SpaceTimeCell>* GetPlan(int agent_ID) const;

    void MoveTime(int delta_time);

    int NumOfGoalsReached() const;

    FTYPE GetDepth() const;
};
