#pragma once

#include <fstream>
#include <vector>
#include "agents.h"

class ScenariesCollection
{
protected:
    std::vector<AgentTask> tasks_;
    std::vector<float> optimal_lengths_;

public:
    void PrepareScenaries(std::ifstream& scenaries_file);
    
    int GetNum() const;

    AgentTask GetScenary(size_t scenary_index) const;
};
