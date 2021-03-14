#include "whca.h"

WHCA::WHCA() : space_time_solver_(&space_solver_, &reservation_) 
{
    space_solver_.SetDiagonalCost(SpaceTimeSearch::cost);
}

// TODO change the name to reset configuration
void WHCA::SetConfiguration(const Map* map, const Config& config)
{
    tasks_.clear();
    paths_.clear();
    agents_.Clear();

    space_solver_.SetConfiguration(map, config);
    space_time_solver_.SetConfiguration(map, config);

    map_ = map;
    config_ = &config;
}

int WHCA::AddAgent(AgentTask<SpaceTimeCell> task)
{
    assert(task.start.t >= 0);
    assert(task.goal.t == -1 || task.goal.t >= 0);
    assert(map_ && config_);

    int new_agent = agents_.ReserveNewID();
    tasks_.insert({ new_agent, task });
    results_.insert({ new_agent, SearchResult<SpaceTimeCell>()});
    paths_.insert({ new_agent, {} });

    return new_agent;
}

void WHCA::RemoveAgent(int agent_ID)
{
    assert(agents_.IsStored(agent_ID));

    // TODO Clear reservation table enterances

    tasks_.erase(agent_ID);
    paths_.erase(agent_ID);
    results_.erase(agent_ID);
    agents_.RemoveID(agent_ID);
}

bool WHCA::Plan()
{
    // Clear the reservation table
    reservation_.clear();
   
    // Current start time is considered to be 0
    extra_time_ = 0;

    // All paths are reset
    on_goal_ = 0;

    // Build a single-agent path for each agent
    auto IDs = agents_.GetIDs();
    for (int agent_ID : IDs)
    {
        AgentTask<SpaceTimeCell> task = tasks_.at(agent_ID);

        // Check if we need to build a path on this depth
        if (task.start.t > depth_ && depth_ != -1) continue;

        // Plan in 2d to set heuristic values and check if the path exists at all
        GridCell start = { task.start.i, task.start.j };
        GridCell goal = { task.goal.i, task.goal.j };
        if (nullptr == space_solver_.Plan({ goal, start }))
        {
            reservation_.clear();
            return false;
        }

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

        // Check for success
        if (nullptr == node)
        {
            reservation_.clear();
            return false;
        }

        // Collect the path's data and write it in the 
        space_time_solver_.BuildPathTo(node->cell);
        space_time_solver_.WritePath();

        // Copy the found result
        results_[agent_ID] = space_time_solver_.GetResult();
        SearchResult<SpaceTimeCell>& result = results_[agent_ID];

        // Move the found path from the solver to paths_
        paths_[agent_ID] = std::move(*result.lppath);
        // First node must be in the end
        std::reverse(paths_[agent_ID].begin(), paths_[agent_ID].end());
        // Make sure that result struct points to correct vectors
        result.lppath = &paths_[agent_ID];
        result.hppath = nullptr;

        // Manually check if the goal was reached
        SpaceTimeCell final_cell = paths_[agent_ID].front().cell;
        if (final_cell.i == goal.i && final_cell.j == goal.j)
        {
            if (task.goal.t == -1 || task.goal.t == final_cell.t) ++on_goal_;
        }
    }

    return true;
}

void WHCA::SetDepth(FTYPE new_depth)
{
    assert(new_depth == -1 || new_depth > 0);
    depth_ = new_depth;
}

SearchResult<SpaceTimeCell> WHCA::GetPlan(int agent_ID) const
{
    assert(agents_.IsStored(agent_ID));

    return results_.at(agent_ID);
}

void WHCA::MoveTime(int delta_time)
{
    if (delta_time <= 0) return;

    extra_time_ += delta_time;

    auto IDs = agents_.GetIDs();
    for (int agent_ID : IDs)
    {
        AgentTask<SpaceTimeCell>& task = tasks_[agent_ID];
        std::vector<Node<SpaceTimeCell>>& single_path = paths_[agent_ID];

        // Remove all nodes, which time is in past and present
        while (!single_path.empty() && single_path.back().cell.t <= extra_time_)
        {
            task.start = single_path.back().cell;
            single_path.pop_back();
        }

        // Even if the start wasn't changed we need to decrease its time
        task.start.t -= delta_time;
        if (task.goal.t != -1) task.goal.t -= delta_time;
        // TODO check if goal and start are still valid
    }
}

int WHCA::NumOfGoalsReached() const
{
    return on_goal_;
}

FTYPE WHCA::GetDepth() const
{
    return depth_;
}

const std::unordered_set<SpaceTimeCell>* WHCA::GetResTable() const
{
    return &reservation_;
}
