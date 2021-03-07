#include "astar3d.h"
#include <algorithm>

const FTYPE SpaceTimeSearch::cost = 1;

void SpaceTimeSearch::SetHeuristic(NodeType& node_to_edit)
{
    if (node_to_edit.h >= 0)
    {
        // Not to change already calculated heuristic.
        return;
    }

    GridCell space_cell = { node_to_edit.cell.i, node_to_edit.cell.j };

    heuristic_->ContinueSearch(space_cell);
    node_to_edit.h = heuristic_->GetGValue(space_cell); // check for -1??
}

void SpaceTimeSearch::ExpandNode(NodeType* node_to_expand)
{
    SpaceTimeCell cell_of_node = node_to_expand->cell;
    int i = cell_of_node.i;
    int j = cell_of_node.j;
    int t = cell_of_node.t;
    ExpandNodeMove(node_to_expand, { i + 1, j + 1, t + 1 } );
    ExpandNodeMove(node_to_expand, { i + 1, j,     t + 1 } );
    ExpandNodeMove(node_to_expand, { i + 1, j - 1, t + 1 });
    ExpandNodeMove(node_to_expand, { i,     j + 1, t + 1 } );
    ExpandNodeMove(node_to_expand, { i,     j,     t + 1 } );
    ExpandNodeMove(node_to_expand, { i,     j - 1, t + 1 } );
    ExpandNodeMove(node_to_expand, { i - 1, j + 1, t + 1 } );
    ExpandNodeMove(node_to_expand, { i - 1, j,     t + 1 } );
    ExpandNodeMove(node_to_expand, { i - 1, j - 1, t + 1 } );
}

FTYPE SpaceTimeSearch::GetMoveCost(SpaceTimeCell from, SpaceTimeCell to) const
{
    // Check the reservation table
    if (reservation_->find(to) != reservation_->end())
    {
        // @TODO obstacle avoidance properties
        return -1;
    }

    // Check swapping conflict
    SpaceTimeCell from_future = from;
    from_future.t += 1;

    auto iterator = reservation_->find(from_future);
    Move conflict_move = SpaceTimeSearch::GetMove(to, from_future);
    if (iterator != reservation_->end() && iterator->move == conflict_move) return -1;

    if (!map_->IsCellOnGrid(to.i, to.j) ||
        !map_->IsCellTraversable(to.i, to.j))
        return -1;

    // Check if the current step goes through the diagonal.
    if (!((to.i - from.i) && (to.j - from.j))) return cost;

    if (!options_.allowdiagonal) return -1;

    // Count untraversable cells on the diagonal path.
    char count = (char)!map_->IsCellTraversable(from.i, to.j) + (char)!map_->IsCellTraversable(to.i, from.j);

    // Check if the path through the diagonal is possible.
    if ((count > 0 && !options_.cutcorners) || 
        (count == 2 && !options_.allowsqueeze)) 
        return -1;

    return cost;
}

void SpaceTimeSearch::BuildCompactPath()
{
    if (!lppath_.size()) return;

    hppath_.clear();

    // Create hppath
    int previous_direction[2] = { 100, 100 };
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

SpaceTimeSearch::SpaceTimeSearch(SingleSearch<GridCell>* heuristic, std::unordered_set<SpaceTimeCell>* reservation)
{
    assert(heuristic);
    assert(reservation);
    heuristic_ = heuristic;
    reservation_ = reservation;
}

const Node<SpaceTimeCell>* SpaceTimeSearch::CellIsReached(SpaceTimeCell cell) const
{
    if (cell.t == -1)
    {
        return nullptr;
    }
    else
    {
        return SingleSearch<SpaceTimeCell>::CellIsReached(cell);
    }
}

bool SpaceTimeSearch::NodeReachedCell(NodeType* node, SpaceTimeCell cell, FTYPE depth) const
{
    if (nullptr == node) return false;

    if (depth > 0) return (node->g >= depth);

    if (cell.t == -1)
    {
        return (node->cell.i == cell.i && node->cell.j == cell.j);
    }
    else
    {
        return node->cell == cell;
    }
}

void SpaceTimeSearch::WritePath() const
{
    for (const Node<SpaceTimeCell>& node : lppath_)
    {
        assert(reservation_->find(node.cell) == reservation_->end());
        SpaceTimeCell new_cell = node.cell;
        if (node.parent == nullptr)
        {
            new_cell.move = Spawn;
        }
        else
        {
            new_cell.move = SpaceTimeSearch::GetMove(node.parent->cell, node.cell);
        }

        reservation_->insert(new_cell);
    }
}

Move SpaceTimeSearch::GetMove(SpaceTimeCell from, SpaceTimeCell to)
{
    unsigned char move = 0;
    if (from.i == to.i)
    {
        move = 3;
    }
    else if (from.i > to.i)
    {
        move = 6;
    }

    if (from.j == to.j)
    {
        move += 1;
    }
    else if (from.j > to.j)
    {
        move += 2;
    }

    return static_cast<Move>(move);
}
