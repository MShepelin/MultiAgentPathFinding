#include "tasker.h"
#include <iostream>
#include <cassert>

bool Tasker::PrepareMap(const char* file_name)
{
    std::ifstream file(file_name);
    if (!file.is_open())
    {
        return false;
    }

    bool map_prepared = map_.PrepareMap(file);

    file.close();

    return map_prepared;
}

bool Tasker::PrepareConfig(const char* file_name)
{
    return config_.PrepareConfig(file_name);
}

bool Tasker::PrepareScenaries(const char* file_name)
{
    std::ifstream file(file_name);
    if (!file.is_open())
    {
        return false;
    }

    scenaries_.PrepareScenaries(file);

    file.close();

    return true;
}

int Tasker::GetScenariesNum() const
{
    return scenaries_.GetNum();
}

void Tasker::StartSearch(size_t scenary_ID, std::ostream* log_stream)
{
    assert(scenary_ID < scenaries_.GetNum());

    solver_.SetConfiguration(&map_, config_);

    AgentTask<GridCell> task = scenaries_.GetScenary(scenary_ID);

    AgentTask<SpaceTimeCell> task3d = { 
        {
            task.start.i,
            task.start.j,
            0
        },
        {
            task.goal.i,
            task.goal.j,
            -1
        }
    };

    int agent_ID = solver_.AddAgent(task3d);
    solver_.Plan();
    const std::vector<SpaceTimeCell>* plan = solver_.GetPlan(agent_ID);


    /*
    if (!log_stream) return;

    *log_stream << "Path ";
    if (!single_search_result_.pathfound) *log_stream << "NOT ";
    *log_stream << "found!\n";
    *log_stream << "numberofsteps=" << single_search_result_.numberofsteps << "\n";
    *log_stream << "nodescreated=" << single_search_result_.nodescreated << "\n";
    if (single_search_result_.pathfound) 
    {
        *log_stream << "pathlength=" << single_search_result_.pathlength << "\n";
        *log_stream << "pathlength_scaled=" << single_search_result_.pathlength << "\n";
    }
    *log_stream << "time=" << single_search_result_.time << "\n";

    // Optimal length is calculated for the cases where "agents cannot cut corners through walls"
    //if (solver_->GetEnvironmentOptions().allowdiagonal == true) return;

    *log_stream << "\nOptimal path length = " << scenaries_.GetOptimalLength(scenary_ID) << "\n";*/
}

void Tasker::StartSearch(size_t first_scenary_ID, size_t last_scenary_ID, std::ostream* log_stream)
{
    assert(first_scenary_ID <= last_scenary_ID);
    assert(last_scenary_ID < scenaries_.GetNum());

    std::vector<AgentTask<GridCell>> tasks;
    std::vector<int> agent_IDs;

    int move_steps = solver_.GetDepth();
    if (move_steps > 0) move_steps /= 2;

    size_t scenary_ID = first_scenary_ID;
    for (; scenary_ID <= last_scenary_ID; ++scenary_ID)
    {
        // Log progress
        *log_stream << "Scenarios: " + std::to_string(first_scenary_ID) + "-" + std::to_string(scenary_ID) << "\n";

        // Create new stats
        /*
        stats_.emplace_back(Stats{
            ((scenary_ID == first_scenary_ID) ? scenaries_.GetMapName() + ", scenarios: " : "Scenarios: ") +
                std::to_string(first_scenary_ID) + "-" + std::to_string(scenary_ID),
            0,
            0,
            scenary_ID - first_scenary_ID + 1 });*/

        // locs will be used to print map
        std::vector<GridCell> locs;

        // Set up a problem's conditions
        tasks.push_back(scenaries_.GetScenary(scenary_ID));
        agent_IDs.clear();

        solver_.SetConfiguration(&map_, config_);

        for (AgentTask<GridCell> task : tasks)
        {
            AgentTask<SpaceTimeCell> task3d = {
                {
                    task.start.i,
                    task.start.j,
                    0
                },
                {
                    task.goal.i,
                    task.goal.j,
                    -1
                }
            };

            // Add task
            agent_IDs.emplace_back(solver_.AddAgent(task3d));

            // Add start location
            locs.push_back(task.start);
        }

        int goals_reached = 0;
        int start_time = 1;

        // Print start map
        PrintMap(locs, log_stream);

        while (goals_reached < agent_IDs.size())
        {
            // Plan
            if (!solver_.Plan())
            {
                *log_stream << "Plan failed\n";
                return;
            }

            std::vector<std::vector<SpaceTimeCell>> paths;

            for (int agent_index = 0; agent_index < scenary_ID - first_scenary_ID + 1; ++agent_index)
            {
                paths.push_back(*solver_.GetPlan(agent_IDs[agent_index]));
            }

            bool repeat = true;

            // We don't print first time unit
            int t = 1;

            goals_reached = solver_.NumOfGoalsReached();

            while(repeat)
            {
                repeat = false;
                locs.clear();

                for (std::vector<SpaceTimeCell>& path : paths)
                {
                    if (!path.empty() && path.back().t <= t)
                    {
                        repeat = true;
                        locs.push_back({ path.back().i, path.back().j });

                        while (!path.empty() && path.back().t <= t)
                        {
                            if (path.back().t > move_steps && goals_reached < agent_IDs.size())
                            {
                                repeat = false;
                                break;
                            }
                            
                            locs.back() = { path.back().i, path.back().j };
                            path.pop_back();
                        }
                    }
                }

                if (!repeat) break;

                *log_stream << "Time " << t - 1 + start_time << "\n";

                PrintMap(locs, log_stream);

                ++t;
            }

            start_time += t - 1;

            if (move_steps > 0) solver_.MoveTime(move_steps);
        }

        // Gather output
        /*
        for (int agent_index = 0; agent_index < scenary_ID - first_scenary_ID + 1; ++agent_index)
        {
            SearchResult<GridCell> result = solver_.GetPlan(agent_IDs[agent_index]);
            stats_.back().total_time += result.time;
            stats_.back().length_delta += result.pathlength - scenaries_.GetOptimalLength(first_scenary_ID + agent_index);
        }*/
    }
}

void Tasker::PrintStatistics(std::ostream* log_stream) const
{
    for (Stats stats_measuring : stats_)
    {
        *log_stream << stats_measuring.desription << "\n";
        *log_stream << "Total computation time = " << stats_measuring.total_time << "\n";
        *log_stream << "On average, planned paths differ by " << 
            stats_measuring.length_delta / stats_measuring.number_of_agents << " from the shortest distances\n";
        *log_stream << "\n";
    }
}

const Map* Tasker::GetMap() const
{
    return &map_;
}

void Tasker::ClearStatistics()
{
    stats_.clear();
}

WHCA* Tasker::GetSolver()
{
    return &solver_;
}

void Tasker::PrintMap(std::vector<GridCell>& locs, std::ostream* log_stream) const
{
    for (int i = 0; i < map_.GetHeight(); ++i)
    {
        for (int j = 0; j < map_.GetWidth(); ++j)
        {
            bool found = false;
            for (int cell_index = 0; cell_index < locs.size(); cell_index++)
            {
                if (locs[cell_index].i == i && locs[cell_index].j == j)
                {
                    *log_stream << (char)('0' + cell_index);
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                if (map_.IsCellTraversable(i, j))
                {
                    *log_stream << ".";
                }
                else
                {
                    *log_stream << '#';
                }
            }
        }

        *log_stream << "\n";
    }
}
