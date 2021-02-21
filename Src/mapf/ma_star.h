#pragma once

#include <cassert>
#include "agents.h"
#include "agents_array.h"
#include "mapf_interface.h"
#include "search.h"
#include <vector>
#include <unordered_map>

class MAStar : public MAPFSolverInterface<GridCell>
{
protected:
    std::unordered_map<int, GridSingleSearch> single_solvers_;
    std::unordered_map<int, AgentTask<GridCell>> tasks_;
    AgentsArray<GridCell> agents_;

    const Map* map_ = nullptr;
    const Config* config_ = nullptr;

public:
    virtual void SetConfiguration(const Map* map, const Config& config) override;

    virtual int AddAgent(AgentTask<GridCell> task) override;

    virtual void RemoveAgent(int agent_ID) override;

    virtual void Plan() override;

    virtual SearchResult<GridCell> GetPlan(int agent_ID) const override;
};
