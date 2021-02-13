#include "agents_array.h"

int AgentsArray::ReserveNewID()
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

void AgentsArray::RemoveID(int agent_ID)
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

bool AgentsArray::PushResult(int agent_ID, SearchResult result)
{
    if (agent_ID < 0 || agent_ID >= results_.size() || !storage_validation_[agent_ID])
    {
        return false;
    }

    results_[agent_ID] = result;
}

bool AgentsArray::PushTask(int agent_ID, AgentTask task)
{
    if (agent_ID < 0 || agent_ID >= tasks_.size() || !storage_validation_[agent_ID])
    {
        return false;
    }

    tasks_[agent_ID] = task;
}

SearchResult AgentsArray::GetResult(int agent_ID) const
{
    if (agent_ID < 0 || agent_ID >= storage_validation_.size() || !storage_validation_[agent_ID])
    {
        throw std::out_of_range("Agent ID is incorrect");
    }

    return results_[agent_ID];
}

AgentTask AgentsArray::GetTask(int agent_ID) const
{
    if (agent_ID < 0 || agent_ID >= storage_validation_.size() || !storage_validation_[agent_ID])
    {
        throw std::out_of_range("Agent ID is incorrect");
    }

    return tasks_[agent_ID];
}

std::vector<int> AgentsArray::GetIDs() const
{
    std::vector<int> IDs;
    for (int i = 0; i < storage_validation_.size(); ++i)
    {
        if (storage_validation_[i]) IDs.push_back(i);
    }

    return IDs;
}

bool AgentsArray::IsStored(int agent_ID)
{
    if (agent_ID < 0 || agent_ID >= storage_validation_.size() || !storage_validation_[agent_ID])
    {
        return false;
    }

    return true;
}