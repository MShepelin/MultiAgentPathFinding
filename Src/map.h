#pragma once

#include <fstream>

class Map
{
protected:
    int height_, width_;
    int **grid_;

    void ClearGrid();

public:
    Map();
    virtual ~Map();

    bool PrepareMap(std::ifstream& map_file);

    // Checks if the [i, j] pair is traversable, considering that [i, j] is on grid
    virtual bool IsCellTraversable(int i, int j) const;

    bool IsCellOnGrid(int i, int j) const;

    int GetHeight() const;

    int GetWidth() const;
};
