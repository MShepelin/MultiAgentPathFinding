#include "tasker.h"
#include <iostream>
#include <cassert>
#include <stdlib.h> 

#define TIME_LIMIT 1000

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

void Tasker:: StartSearch(size_t scenary_ID, std::ostream* log_stream)
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

    SearchResult<SpaceTimeCell> result = solver_.GetPlan(agent_ID);

    if (!log_stream) return;

    *log_stream << "Path ";
    if (!result.pathfound) *log_stream << "NOT ";
    *log_stream << "found!\n";
    *log_stream << "numberofsteps=" << result.numberofsteps << "\n";
    *log_stream << "nodescreated=" << result.nodescreated << "\n";
    if (result.pathfound)
    {
        *log_stream << "pathlength=" << result.pathlength << "\n";
        *log_stream << "pathlength_scaled=" << result.pathlength << "\n";
    }
    *log_stream << "time=" << result.time << "\n";

    // Usually optimal length is calculated for the cases where "agents can move diagonally but cannot cut corners"
    *log_stream << "\nOptimal path length from file = " << scenaries_.GetOptimalLength(scenary_ID) << "\n";
}

void Tasker::StartSearch(size_t first_scenary_ID, size_t last_scenary_ID, std::ostream* log_stream)
{
    assert(first_scenary_ID <= last_scenary_ID);
    assert(last_scenary_ID < scenaries_.GetNum());

    Checker check;

    std::vector<AgentTask<GridCell>> tasks;
    std::vector<int> agent_IDs;

    int move_steps = solver_.GetDepth();

    // TODO support depth == -1
    assert(move_steps > 0);
    move_steps /= 2;

    size_t scenary_ID = first_scenary_ID;

    if (options_.use_all_scenarios)
    {
        for (size_t scenary = first_scenary_ID; scenary < last_scenary_ID; ++scenary)
        {
            tasks.push_back(scenaries_.GetScenary(scenary));
        }
        scenary_ID = last_scenary_ID;
    }
    
    for (; scenary_ID <= last_scenary_ID; ++scenary_ID)
    {
        // Log progress
        if (options_.print_markers)
        {
            *log_stream << "Scenarios: " + std::to_string(first_scenary_ID) + "-" + std::to_string(scenary_ID) << "\n";
        }

        // Create new stats
        stats_.emplace_back(Stats{
            ((scenary_ID == first_scenary_ID) ? scenaries_.GetMapName() + ", scenarios: " : "Scenarios: ") +
                std::to_string(first_scenary_ID) + "-" + std::to_string(scenary_ID),
            0,
            0,
            scenary_ID - first_scenary_ID + 1 });

        // locs will be used to print map
        std::vector<GridCell> locs;

        // Set up a problem's conditions
        tasks.push_back(scenaries_.GetScenary(scenary_ID));
        agent_IDs.clear();

        // Setup the solver
        solver_.SetConfiguration(&map_, config_);
        check.SetConfiguration(&map_, config_);

        // Add tasks
        for (AgentTask<GridCell> task : tasks)
        {
            AgentTask<SpaceTimeCell> task3d = {
                {
                    task.start.i,
                    task.start.j,
                    (options_.random_start) ? rand() % tasks.size() : 0
                },
                {
                    task.goal.i,
                    task.goal.j,
                    -1
                }
            };

            agent_IDs.emplace_back(solver_.AddAgent(task3d));

            if (task3d.start.t > 0)
            {
                locs.push_back(task.start);
            }
            else
            {
                locs.push_back({ -1, -1 });
            }
        }

        int goals_reached = 0;
        int start_time = 1;

        // Print start map
        if (options_.log_level >= LOG_PRINT_FRAMES)
        {
            PrintMap(locs, log_stream);
        }

        while (goals_reached < agent_IDs.size())
        {
            // Plan
            if (!solver_.Plan())
            {
                if (options_.print_markers) *log_stream << "Plan failed\n";
                return;
            }

            // Check
            if (options_.check_solution)
            {
                check.CheckNaive(solver_.GetResTable(), log_stream);
            }

            goals_reached = solver_.NumOfGoalsReached();

            int target_time = (goals_reached == agent_IDs.size()) ? solver_.GetDepth() : move_steps;

            if (options_.log_level >= LOG_PRINT_FRAMES)
            {
                // Print each time unit except for the first
                for (int i = 1; i <= target_time; ++i)
                {
                    locs.clear();

                    for (int ID : agent_IDs)
                    {
                        // Check if plan exists
                        auto plan = solver_.GetPlan(ID).lppath;
                        if (!plan)
                        {
                            locs.push_back({ -1, -1 });
                            continue;
                        }

                        // Check if the start of the plan has the needed time
                        int start_t = plan->back().cell.t;
                        if (start_t > i)
                        {
                            locs.push_back({ -1, -1 });
                            continue;
                        }
                        
                        Node<SpaceTimeCell> node = plan->at(solver_.GetDepth() - i - start_t);
                        locs.push_back({ node.cell.i, node.cell.j });
                    }

                    if (options_.print_markers)
                    {
                        *log_stream << "Time " << i - 1 + start_time << "\n";
                    }

                    PrintMap(locs, log_stream);
                }
            }
            else if (options_.log_level >= LOG_PRINT_PATHS)
            {
                // Print each path
                for (int ID : agent_IDs)
                {
                    if (options_.print_markers) *log_stream << "Agent " << ID << "\n";

                    locs.clear();
                    auto plan = solver_.GetPlan(ID).lppath;
                    if (!plan) continue;
                    for (const Node<SpaceTimeCell> &node : *plan)
                    {
                        locs.push_back({ node.cell.i, node.cell.j });
                    }
                    
                    PrintPath(locs, log_stream);
                }
            }

            solver_.MoveTime(move_steps);
            start_time += move_steps;

            if (start_time > TIME_LIMIT)
            {
                if (options_.print_markers) *log_stream << "TOO LONG\n";
                break;
            }
        }

        // Gather stats
        for (int agent_index = 0; agent_index < scenary_ID - first_scenary_ID + 1; ++agent_index)
        {
            SearchResult<SpaceTimeCell> result = solver_.GetPlan(agent_IDs[agent_index]);
            stats_.back().total_time += result.time;
            stats_.back().length_delta += result.pathlength - scenaries_.GetOptimalLength(first_scenary_ID + agent_index);
        }
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
    std::vector<std::vector<char>> print(map_.GetHeight(), std::vector<char>(map_.GetWidth(), '.'));
    for (size_t cell_index = 0; cell_index < locs.size(); cell_index++)
    {
        if (locs[cell_index].i < 0 || locs[cell_index].j < 0) continue;

        print[locs[cell_index].i][locs[cell_index].j] = 'A' + cell_index;
    }

    for (int i = 0; i < map_.GetHeight(); ++i)
    {
        for (int j = 0; j < map_.GetWidth(); ++j)
        {
            if (!map_.IsCellTraversable(i, j))
            {
                *log_stream << "@";
            }
            else
            {
                *log_stream << print[i][j];
            }
        }

        *log_stream << "\n";
    }
}

void Tasker::PrintPath(std::vector<GridCell>& locs, std::ostream* log_stream) const
{
    std::vector<std::vector<char>> print(map_.GetHeight(), std::vector<char>(map_.GetWidth(), '.'));
    for (GridCell& cell : locs)
    {
        if (cell.i < 0 || cell.j < 0) continue;

        print[cell.i][cell.j] = '0';
    }

    for (int i = 0; i < map_.GetHeight(); ++i)
    {
        for (int j = 0; j < map_.GetWidth(); ++j)
        {
            if (!map_.IsCellTraversable(i, j))
            {
                *log_stream << "@";
            }
            else
            {
                *log_stream << print[i][j];
            }
        }

        *log_stream << "\n";
    }
}

void Tasker::SetOptions(TaskerOptions options)
{
    options_ = options;
}
