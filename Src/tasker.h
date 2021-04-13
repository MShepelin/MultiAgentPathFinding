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
    bool path_found = true;
    bool check = true;
};

#define NO_PLAN_LOG 0
#define LOG_RAW_PATHS 1
#define LOG_PATHS_FRAMES 2
#define LOG_FULL_FRAMES 3

struct TaskerOptions
{
    // States whether we need to check the correctness of solution or not
    unsigned int check_solution : 1;
    // We can either add scenarios gradually or use all given task in the same time.
    // To use the second option use_all_scenarios should be true
    unsigned int use_all_scenarios : 1;
    // Markers give information about current set of scenarios and time frames
    unsigned int print_markers : 1;
    unsigned int random_start : 1;
    
    int log_level;
};

class Tasker
{
protected:
    Map map_;
    Config config_;
    ScenariesCollection scenaries_;
    TaskerOptions options_;
    WHCA solver_;

    std::vector<Stats> stats_;
    std::unordered_map<int, std::vector<SpaceTimeCell>> raw_paths_;

public:
    void SetOptions(TaskerOptions options);

    bool PrepareMap(const char* file_name);
    bool PrepareConfig(const char* file_name);
    bool PrepareScenaries(const char* file_name);

    void StartSearch(size_t scenary_ID, std::ostream* log_stream);
    void StartSearch(size_t first_scenary_ID, size_t last_scenary_ID, std::ostream* log_stream);

    void PrintMap(std::vector<GridCell>& locs, std::ostream* log_stream) const;
    void PrintPath(std::vector<GridCell>& locs, std::ostream* log_stream) const;
    void PrintStatistics(std::ostream* log_stream) const;

    void ClearStatistics();

    const Map* GetMap() const;
    int GetScenariesNum() const;
    WHCA* GetSolver();
    const std::vector<Stats>* GetStatistics() const;
};
