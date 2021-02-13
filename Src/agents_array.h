#pragma once

#include <vector>
#include "agents.h"
#include "searchresult.h"

class AgentsArray
{
private:
    std::vector<bool> storage_validation_;
    std::vector<SearchResult> results_;
    std::vector<AgentTask> tasks_;
    std::vector<int> popped_IDs_;

public:
    int ReserveNewID();

    void RemoveID(int agent_ID);

    bool PushResult(int agent_ID, SearchResult result);

    bool PushTask(int agent_ID, AgentTask task);

    SearchResult GetResult(int agent_ID) const;

    AgentTask GetTask(int agent_ID) const;

    std::vector<int> GetIDs() const;

    bool IsStored(int agent_ID);
};
