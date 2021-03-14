#pragma once

#include "map.h"
#include "environmentoptions.h"
#include "config.h"
#include <unordered_set>
#include "nodes.h"

// Checks the correctness of actions in reservation table
class Checker
{
protected:
    const Map* map_ = nullptr;
    EnvironmentOptions options_;

public:
    void SetConfiguration(const Map* map, const Config& config);

    bool CheckNaive(const std::unordered_set<SpaceTimeCell>* reservation, std::ostream* log_stream); 
};
