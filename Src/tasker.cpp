#include "tasker.h"
#include <iostream>
#include <cassert>

void Tasker::SetSolver(MAPFSolverInterface<GridCell>* new_solver)
{
    solver_ = new_solver;
}

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
    assert(solver_);

    //solver_->Clear();
    solver_->SetConfiguration(&map_, config_);

    AgentTask<GridCell> task = scenaries_.GetScenary(scenary_ID);

    int agent_ID = solver_->AddAgent(task);
    solver_->Plan();
    single_search_result_ = solver_->GetPlan(agent_ID);

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

    *log_stream << "\nOptimal path length = " << scenaries_.GetOptimalLength(scenary_ID) << "\n";
}

void Tasker::StartSearch(size_t first_scenary_ID, size_t last_scenary_ID)
{
    assert(first_scenary_ID <= last_scenary_ID);
    assert(last_scenary_ID < scenaries_.GetNum());

    //TODO
}

void Tasker::PrintStatistics(std::ofstream* log_stream)
{
    //TODO
}

const Map* Tasker::GetMap() const
{
    return &map_;
}
