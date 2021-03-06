#include "whca.h"

WHCA::WHCA() : space_time_solver_(&space_solver_, &reservation_) {}

void WHCA::SetConfiguration(const Map* map, const Config& config)
{
    space_solver_.SetConfiguration(map, config);
    space_time_solver_.SetConfiguration(map, config);

    map_ = map;
    config_ = &config;
}

int WHCA::AddAgent(AgentTask<SpaceTimeCell> task)
{
    assert(map_ && config_);
    // @TODO check task

    int new_agent = agents_.ReserveNewID();
    tasks_.insert({ new_agent, task });
    paths_.insert({ new_agent, std::stack<SpaceTimeCell>()});

    return new_agent;
}

void WHCA::RemoveAgent(int agent_ID)
{
    assert(agents_.IsStored(agent_ID));

    tasks_.erase(agent_ID);
    paths_.erase(agent_ID);
    agents_.RemoveID(agent_ID);
}

void WHCA::Plan()
{
    // Clear the reservation table
    reservation_.clear();

    // @note Current start time is considered to be 0

    for (int agent_ID : agents_.GetIDs())
    {
        // Build a single path
        AgentTask<SpaceTimeCell> task = tasks_.at(agent_ID);

        if (task.start.t > depth_) continue;

        GridCell start = { task.start.i, task.start.j };
        GridCell goal = { task.goal.i, task.goal.j };
        space_solver_.Plan({ start, goal });

        // @TODO Check if it exists

        // Run 3d pathfinder
        const Node<SpaceTimeCell>* node;
        if (depth_ == -1)
        {
            node = space_time_solver_.Plan(task, depth_);
        }
        else
        {
            node = space_time_solver_.Plan(task, depth_ - task.start.t);
        }

        space_time_solver_.BuildPathTo(node->cell);
        space_time_solver_.WritePath();

        // Copy the found path
        std::stack<SpaceTimeCell>& single_path = paths_[agent_ID];
        while (!single_path.empty()) paths_[agent_ID].pop();
        
        SearchResult<SpaceTimeCell> result = space_time_solver_.GetResult();

        for (size_t node_index = result.lppath->size(); node_index > 0; --node_index)
        {
            single_path.push(result.lppath->at(node_index - 1).cell);
        }
    }
}

void WHCA::SetDepth(FTYPE new_depth)
{
    depth_ = new_depth;
}

const std::stack<SpaceTimeCell>* WHCA::GetPlan(int agent_ID) const
{
    assert(agents_.IsStored(agent_ID));

    return &paths_.at(agent_ID);
}

void WHCA::MoveTime(int delta_time)
{
    extra_time_ += delta_time;

    for (int agent_ID : agents_.GetIDs())
    {
        AgentTask<SpaceTimeCell>& task = tasks_[agent_ID];
        std::stack<SpaceTimeCell>& single_path = paths_[agent_ID];

        while (!single_path.empty() && single_path.top().t <= extra_time_)
        {
            task.start = single_path.top();
            single_path.pop();
        }

        task.start.t = 0;
        task.goal.t -= delta_time;
    }
}
