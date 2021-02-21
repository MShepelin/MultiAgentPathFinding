#pragma once

#include "map.h"
#include "config.h"
#include "searchresult.h"
#include "mapf/mapf_interface.h"
#include "scenarios.h"
#include "nodes.h"
#include <vector>
#include <fstream>

class Tasker
{
protected:
    Map map_;
    Config config_;
    ScenariesCollection scenaries_;
    SearchResult<GridCell> single_search_result_;
    std::vector<SearchResult<GridCell>> results_;
    MAPFSolverInterface<GridCell>* solver_ = nullptr;

public:
    void SetSolver(MAPFSolverInterface<GridCell>* new_solver);

    bool PrepareMap(const char* file_name);
    bool PrepareConfig(const char* file_name);
    bool PrepareScenaries(const char* file_name);

    int GetScenariesNum() const;

    void StartSearch(size_t scenary_ID, std::ostream* log_stream = nullptr);

    void StartSearch(size_t first_scenary_ID, size_t last_scenary_ID);

    void PrintStatistics(std::ofstream* log_stream);

    const Map* GetMap() const;
};
