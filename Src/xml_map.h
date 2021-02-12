#pragma once

#include <iostream>
#include "gl_const.h"
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include "tinyxml2.h"

class XMLMap
{
private:
    int     height_, width_;
    int     start_i, start_j;
    int     goal_i, goal_j;
    double  cell_size_;
    int**   grid_;

public:
    XMLMap();
    XMLMap(const XMLMap& orig);
    ~XMLMap();

    bool PrepareMap(const char *FileName);
    bool IsCellTraversable(int i, int j) const;
    bool IsCellOnGrid(int i, int j) const;
    int getValue(int i, int j) const;
    int GetHeight() const;
    int GetWidth() const;
    double GetCellSize() const;
    void GetTask(int* buffer) const;
};
