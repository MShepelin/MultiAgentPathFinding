#pragma once

#include "agents.h"

struct PlanOptions
{
    int d;
};

class MAPFSolverInterface
{
    using AgentIDType = int;

public:
    virtual AgentIDType AddAgent(AgentTask task) = 0;

    virtual void RemoveAgent(AgentIDType agent_ID) = 0;

    virtual void Plan(PlanOptions options, bool full_plan = true) = 0;

    virtual void GetPlan(AgentIDType agent_ID) = 0;
};