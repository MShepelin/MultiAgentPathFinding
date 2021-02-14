#pragma once

#include "map.h"
#include <iostream>
#include "gl_const.h"
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include "tinyxml2.h"
#include "agents.h"
#include "nodes.h"

class XMLMap : public Map
{
protected:
    int     start_i, start_j;
    int     goal_i, goal_j;
    double  cell_size_;
    int**   grid_;

public:
    XMLMap();

    XMLMap(const XMLMap& orig);

    ~XMLMap();

    bool PrepareMap(const char *FileName);

    virtual bool IsCellTraversable(int i, int j) const override;

    int getValue(int i, int j) const;

    double GetCellSize() const;

    AgentTask<GridCell> GetTask() const;
};
