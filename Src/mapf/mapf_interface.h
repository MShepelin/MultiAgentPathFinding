#pragma once

#include "agents.h"
#include "searchresult.h"
#include "environmentoptions.h"
#include "config.h"
#include "map.h"

template<typename CellType>
class MAPFSolverInterface
{
public:
    virtual void SetConfiguration(const Map* map, const Config& config) = 0;

    virtual int AddAgent(AgentTask<CellType> task) = 0;

    virtual void RemoveAgent(int agent_ID) = 0;

    virtual void Plan() = 0;

    virtual SearchResult<CellType> GetPlan(int agent_ID) const = 0;
};
