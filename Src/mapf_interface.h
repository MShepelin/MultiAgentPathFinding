#pragma once

#include "agents.h"
#include "searchresult.h"
#include "environmentoptions.h"
#include "config.h"
#include "map.h"

class MAPFSolverInterface
{
protected:
    using AgentIDType = int;

public:
    virtual void SetConfiguration(const Map* map, const EnvironmentOptions &options, const Config& config) = 0;

    virtual AgentIDType AddAgent(AgentTask task) = 0;

    virtual void RemoveAgent(AgentIDType agent_ID) = 0;

    virtual void Plan(bool full_plan = true) = 0;

    virtual SearchResult GetPlan(AgentIDType agent_ID) const = 0;
};
