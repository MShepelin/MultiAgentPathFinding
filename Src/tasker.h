#pragma once

#include "map.h"
#include "config.h"
#include "searchresult.h"
#include "mapf/whca.h"
#include "scenarios.h"
#include "nodes.h"
#include <vector>
#include <fstream>
#include "mapf/checker.h"

struct Stats
{
    std::string desription;
    double total_time = 0;
    // Total length delta
    float length_delta = 0;
    size_t number_of_agents = 0;
};

class Tasker
{
protected:
    Map map_;
    Config config_;
    ScenariesCollection scenaries_;

    WHCA solver_;

    std::vector<Stats> stats_;

    void PrintMap(std::vector<GridCell>& locs, std::ostream* log_stream) const;

public:
    bool PrepareMap(const char* file_name);
    bool PrepareConfig(const char* file_name);
    bool PrepareScenaries(const char* file_name);

    int GetScenariesNum() const;

    void StartSearch(size_t scenary_ID, std::ostream* log_stream);

    void StartSearch(size_t first_scenary_ID, size_t last_scenary_ID, std::ostream* log_stream, bool check_solution = true);

    void PrintStatistics(std::ostream* log_stream) const;

    void ClearStatistics();

    const Map* GetMap() const;

    WHCA* GetSolver();
};
