#pragma once

#include <vector>
#include "agents.h"
#include "searchresult.h"

template<typename CellType>
class AgentsArray
{
public:
    using ResultType = SearchResult<CellType>;
    using TaskType = AgentTask<CellType>;

private:
    std::vector<bool> storage_validation_;
    std::vector<ResultType> results_;
    std::vector<TaskType> tasks_;
    std::vector<int> popped_IDs_;

public:
    int ReserveNewID();

    void RemoveID(int agent_ID);

    bool SetResult(int agent_ID, ResultType result);

    bool SetTask(int agent_ID, TaskType task);

    ResultType GetResult(int agent_ID) const;

    TaskType GetTask(int agent_ID) const;

    std::vector<int> GetIDs() const;

    bool IsStored(int agent_ID);
};


template<typename CellType>
int AgentsArray<CellType>::ReserveNewID()
{
    if (popped_IDs_.size())
    {
        int available_ID = popped_IDs_.back();
        storage_validation_[available_ID] = true;
        popped_IDs_.pop_back();
        return available_ID;
    }

    results_.push_back(SearchResult());
    storage_validation_.push_back(true);
    tasks_.push_back(AgentTask());

    return results_.size() - 1;
}

template<typename CellType>
void AgentsArray<CellType>::RemoveID(int agent_ID)
{
    if (agent_ID < 0 || agent_ID >= storage_validation_.size())
    {
        throw std::out_of_range("Agent ID is out of range");
    }

    if (agent_ID + 1 == storage_validation_.size())
    {
        storage_validation_.pop_back();
        results_.pop_back();
        tasks_.pop_back();
        return;
    }

    if (storage_validation_[agent_ID])
    {
        popped_IDs_.push_back(agent_ID);
        storage_validation_[agent_ID] = false;
    }
}

template<typename CellType>
bool AgentsArray<CellType>::SetResult(int agent_ID, ResultType result)
{
    if (agent_ID < 0 || agent_ID >= results_.size() || !storage_validation_[agent_ID])
    {
        return false;
    }

    results_[agent_ID] = result;
}

template<typename CellType>
bool AgentsArray<CellType>::SetTask(int agent_ID, AgentTask<CellType> task)
{
    if (agent_ID < 0 || agent_ID >= tasks_.size() || !storage_validation_[agent_ID])
    {
        return false;
    }

    tasks_[agent_ID] = task;
}

template<typename CellType>
SearchResult<CellType> AgentsArray<CellType>::GetResult(int agent_ID) const
{
    if (agent_ID < 0 || agent_ID >= storage_validation_.size() || !storage_validation_[agent_ID])
    {
        throw std::out_of_range("Agent ID is incorrect");
    }

    return results_[agent_ID];
}

template<typename CellType>
AgentTask<CellType> AgentsArray<CellType>::GetTask(int agent_ID) const
{
    if (agent_ID < 0 || agent_ID >= storage_validation_.size() || !storage_validation_[agent_ID])
    {
        throw std::out_of_range("Agent ID is incorrect");
    }

    return tasks_[agent_ID];
}

template<typename CellType>
std::vector<int> AgentsArray<CellType>::GetIDs() const
{
    std::vector<int> IDs;
    for (int i = 0; i < storage_validation_.size(); ++i)
    {
        if (storage_validation_[i]) IDs.push_back(i);
    }

    return IDs;
}

template<typename CellType>
bool AgentsArray<CellType>::IsStored(int agent_ID)
{
    if (agent_ID < 0 || agent_ID >= storage_validation_.size() || !storage_validation_[agent_ID])
    {
        return false;
    }

    return true;
}
