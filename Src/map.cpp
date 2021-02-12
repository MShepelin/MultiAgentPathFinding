#include "map.h"
#include <string>

Map::Map()
{
    width_ = height_ = 0;
    grid_ = nullptr;
}

Map::~Map()
{
    ClearGrid();
}

void Map::ClearGrid()
{
    if (!grid_) return;

    for (int row = 0; row < height_; ++row)
    {
        delete[] grid_[row];
    }
    delete[] grid_;
    height_ = width_ = 0;
}

bool Map::PrepareMap(std::ifstream& map_file)
{
    std::string buffer;
    std::string type;
    int width, height;

    map_file >> buffer;
    if (buffer != "type") return false;

    map_file >> type;
    if (type != "octile") return false;

    map_file >> buffer >> height >> buffer >> width >> buffer;
    if (buffer != "map") return false;
    if (height < 0 || width < 0) return false;

    ClearGrid();
    grid_ = new unsigned short int*[height];
    height_ = height;
    width_ = width;

    char grid_value;
    unsigned short int grid_cell;
    for (int row = 0; row < height; ++row)
    {
        grid_[row] = new unsigned short int [width];
        for (int column = 0; column < width; ++column)
        {
            map_file.get(grid_value);
            grid_cell = 0;
            if (grid_value == '@' ||
                grid_value == 'O' ||
                grid_value == 'T')
                grid_cell = 1;

            grid_[row][column] = grid_cell;
        }

        // end line
        std::getline(map_file, buffer);
    }

    return true;
}

bool Map::IsCellTraversable(int i, int j) const
{
    return grid_[i][j] == 0;
}

bool Map::IsCellOnGrid(int i, int j) const
{
    return (j >= 0 && j < width_) && (i >= 0 && i < height_);
}

int Map::GetHeight() const
{
    return height_;
}

int Map::GetWidth() const
{
    return width_;
}
