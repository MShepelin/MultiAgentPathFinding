#pragma once

#include "search.h"
#include <chrono>
#include <iostream>
#include <algorithm>

// ------------------------------- //
// GridSingleSearch implementation //
// ------------------------------- //

const FTYPE GridSingleSearch::ch = 1;
const FTYPE GridSingleSearch::cd = CN_SQRT_TWO;

void GridSingleSearch::SetHeuristic(NodeType& node_to_edit)
{
    if (node_to_edit.h >= 0)
    {
        // Not to change already calculated heuristic.
        return;
    }

    if (0 == heuristic_weight_)
    {
        node_to_edit.h = 0;
        return;
    }

    double di = abs(node_to_edit.cell.i - task_.goal.i);
    double dj = abs(node_to_edit.cell.j - task_.goal.j);

    switch (options_.metrictype)
    {
    case CN_SP_MT_DIAG:
        node_to_edit.h = ch * abs(di - dj) + cd * std::min(di, dj);
        break;

    case CN_SP_MT_MANH:
        node_to_edit.h = di + dj;
        break;

    case CN_SP_MT_EUCL:
        node_to_edit.h = std::sqrt(di * di + dj * dj);
        break;

    case CN_SP_MT_CHEB:
        node_to_edit.h = std::max(di, dj);
        break;

    default:
        node_to_edit.h = 0;
        break;
    }

    node_to_edit.h *= heuristic_weight_;
}

void GridSingleSearch::ExpandNode(NodeType* node_to_expand)
{
    GridCell cell_of_node = node_to_expand->cell;
    ExpandNodeMove(node_to_expand, { cell_of_node.i - 1, cell_of_node.j - 1 } );
    ExpandNodeMove(node_to_expand, { cell_of_node.i - 1, cell_of_node.j } );
    ExpandNodeMove(node_to_expand, { cell_of_node.i - 1, cell_of_node.j + 1 } );
    ExpandNodeMove(node_to_expand, { cell_of_node.i,     cell_of_node.j - 1 } );
    ExpandNodeMove(node_to_expand, { cell_of_node.i,     cell_of_node.j + 1 } );
    ExpandNodeMove(node_to_expand, { cell_of_node.i + 1, cell_of_node.j - 1 } );
    ExpandNodeMove(node_to_expand, { cell_of_node.i + 1, cell_of_node.j } );
    ExpandNodeMove(node_to_expand, { cell_of_node.i + 1, cell_of_node.j + 1 } );
}

FTYPE GridSingleSearch::GetMoveCost(GridCell from, GridCell to) const
{
    if (!map_->IsCellOnGrid(to.i, to.j) ||
        !map_->IsCellTraversable(to.i, to.j))
        return -1;

    // Check if the current step goes through the diagonal.
    if (!( (to.i - from.i) && (to.j - from.j) )) return ch;
    if (!options_.allowdiagonal) return -1;

    // Count untraversable cells on the diagonal path.
    char count = (char)!map_->IsCellTraversable(from.i, to.j) + (char)!map_->IsCellTraversable(to.i, from.j);

    // Check if the path through the diagonal is possible.
    if ((count > 0 && !options_.cutcorners) || 
        (count == 2 && !options_.allowsqueeze)) 
        return -1;

    return cd;
}

void GridSingleSearch::BuildCompactPath()
{
    if (!lppath_.size()) return;

    hppath_.clear();

    // Create hppath
    int previous_direction[2] = { 0, 0 };
    int current_direction[2] = { 0, 0 };

    hppath_.push_back(lppath_[0]);
    NodeType last_node = lppath_[0];
    for (size_t i = 1; i < lppath_.size(); ++i)
    {
        current_direction[0] = lppath_[i].cell.i - lppath_[i - 1].cell.i;
        current_direction[1] = lppath_[i].cell.j - lppath_[i - 1].cell.j;

        if (current_direction[0] == previous_direction[0] && current_direction[1] == previous_direction[1])
        {
            hppath_.back() = lppath_[i];
        }
        else
        {
            hppath_.push_back(lppath_[i]);
        }

        previous_direction[0] = current_direction[0];
        previous_direction[1] = current_direction[1];
    }
}
