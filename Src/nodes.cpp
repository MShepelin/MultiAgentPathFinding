#pragma once

#include "nodes.h"

bool GridCell::operator==(const GridCell& other) const
{
    return i == other.i && j == other.j;
}

bool TimeSpaceCell::operator==(const TimeSpaceCell& other) const
{
    return i == other.i && j == other.j && t == other.t;
}
