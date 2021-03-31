#pragma once

#include <fstream>
#include <vector>

class Map
{
protected:
    int height_, width_;
    int **grid_;

    void ClearGrid();

public:
    Map();
    ~Map();

    bool PrepareMap(std::ifstream& map_file);
    bool PrepareMap(std::vector<std::vector<int>>& map_info, int height, int width);

    // Checks if the [i, j] pair is traversable, considering that [i, j] is on grid
    virtual bool IsCellTraversable(int i, int j) const;

    bool IsCellOnGrid(int i, int j) const;

    int GetHeight() const;

    int GetWidth() const;
};
