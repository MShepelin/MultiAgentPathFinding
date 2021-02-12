#pragma once

#include "agents.h"
#include "mapf_interface.h"
#include <fstream>

class Map
{
protected:
    int width_;
    int height_;
    short unsigned int **grid_;

    void ClearGrid();

public:
    Map();
    ~Map();

    bool PrepareMap(std::ifstream& map_file);

    // Checks if the [x, y] pair is traversable, considering that [x, y] is on grid
    bool IsCellTraversable(int x, int y) const;

    bool IsCellOnGrid(int x, int y) const;

    int GetHeight() const;

    int GetWidth() const;
};
