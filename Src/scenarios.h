#pragma once

#include <fstream>
#include <vector>
#include "nodes.h"
#include "agents.h"

class ScenariesCollection
{
protected:
    std::vector<AgentTask<GridCell>> tasks_;
    std::vector<FTYPE> optimal_lengths_;

public:
    void PrepareScenaries(std::ifstream& scenaries_file);
    
    size_t GetNum() const;

    AgentTask<GridCell> GetScenary(size_t scenary_index) const;

    FTYPE GetOptimalLength(size_t scenary_index) const;
};
