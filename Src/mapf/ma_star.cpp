#include "ma_star.h"

void MAStar::SetConfiguration(const Map* map, const Config& config)
{
    for (auto& map_iterator : single_solvers_)
    {
        map_iterator.second.SetConfiguration(map, config);
    }

    map_ = map;
    config_ = &config;
}

int MAStar::AddAgent(AgentTask<GridCell> task)
{
    assert(map_ && config_);

    int new_agent = agents_.ReserveNewID();

    single_solvers_.insert({new_agent, GridSingleSearch()});
    single_solvers_[new_agent].SetConfiguration(map_, *config_);
    tasks_.insert({ new_agent, task });

    return new_agent;
}

void MAStar::RemoveAgent(int agent_ID)
{
    assert(agents_.IsStored(agent_ID));

    single_solvers_.erase(agent_ID);
    agents_.RemoveID(agent_ID);
}

void MAStar::Plan()
{
    for (auto& map_iterator : single_solvers_)
    {
        map_iterator.second.Plan(tasks_[map_iterator.first]);
        map_iterator.second.BuildPathTo(tasks_[map_iterator.first].goal);
    }
}

SearchResult<GridCell> MAStar::GetPlan(int agent_ID) const
{
    assert(agents_.IsStored(agent_ID));

    return single_solvers_.at(agent_ID).GetResult();
}
