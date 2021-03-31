#include "astar3d.h"
#include <algorithm>

const FTYPE SpaceTimeSearch::cost = 1;

void SpaceTimeSearch::SetHeuristic(NodeType& node_to_edit)
{
    // TODO assert if the heuristic_ found the path from goal to start

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
    const TTYPE t = cell_of_node.t + 1;

    // Expand the wait move first
    ExpandNodeMove(node_to_expand, { i,     j,     t });

    // Then non-diagonal moves
    ExpandNodeMove(node_to_expand, { i,     j + 1, t });
    ExpandNodeMove(node_to_expand, { i + 1, j,     t });
    ExpandNodeMove(node_to_expand, { i - 1, j,     t });
    ExpandNodeMove(node_to_expand, { i,     j - 1, t });
    
    // Then diagonal moves
    ExpandNodeMove(node_to_expand, { i + 1, j + 1, t } );
    ExpandNodeMove(node_to_expand, { i + 1, j - 1, t });
    ExpandNodeMove(node_to_expand, { i - 1, j + 1, t } );
    ExpandNodeMove(node_to_expand, { i - 1, j - 1, t } );
}

FTYPE SpaceTimeSearch::GetMoveCost(SpaceTimeCell from, SpaceTimeCell to) const
{
    // Check if to exists on the map
    if (!map_->IsCellOnGrid(to.i, to.j) ||
        !map_->IsCellTraversable(to.i, to.j))
        return -1;

    // Check if the target is an available cell
    if (reservation_->find(to) != reservation_->end()) return -1;

    // Check swapping conflict
    SpaceTimeCell from_future = from;
    from_future.t += 1;

    auto iterator = reservation_->find(from_future);
    Move conflict_move = SpaceTimeSearch::GetMove(to, from_future);
    if (iterator != reservation_->end() && iterator->move == conflict_move) return -1;

    // Check if the current step goes through a diagonal
    if (!((to.i - from.i) && (to.j - from.j)))
    {
        if (IsNondiagonalCollision(from, to)) return -1;
        return cost;
    }

    if (!options_.allowdiagonal) return -1;
    if (IsDiagonalCollision(from, to)) return -1;

    // Check if someone moves through the same point of the cell
    Move move = SpaceTimeSearch::GetMove(from, to);
    const TTYPE t = from.t + 1;
    switch (move)
    {
    case UpLeft:
        if (diagonals_->find({ from.i - 1, from.j, t }) != diagonals_->end()) return -1;
    case UpRight:
        if (diagonals_->find({ from.i - 1, from.j + 1, t }) != diagonals_->end()) return -1;
    case DownLeft:
        if (diagonals_->find({ from.i, from.j, t }) != diagonals_->end()) return -1;
    case DownRight:
        if (diagonals_->find({ from.i, from.j + 1, t }) != diagonals_->end()) return -1;
    }

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

SpaceTimeSearch::SpaceTimeSearch(SingleSearch<GridCell>* heuristic, std::unordered_set<SpaceTimeCell>* reservation, std::unordered_set<SpaceTimeCell>* diagonals)
{
    assert(heuristic);
    assert(reservation);
    assert(diagonals);
    heuristic_ = heuristic;
    reservation_ = reservation;
    diagonals_ = diagonals;
    radius_ = 0.5;
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
    // TODO check if BuildPath was executed
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

        const TTYPE t = new_cell.t + 1;
        switch (new_cell.move)
        {
        case UpLeft:
            diagonals_->insert({ new_cell.i - 1, new_cell.j, t });
            break;
        case UpRight:
            diagonals_->insert({ new_cell.i - 1, new_cell.j + 1, t });
            break;
        case DownLeft:
            diagonals_->insert({ new_cell.i, new_cell.j, t });
            break;
        case DownRight:
            diagonals_->insert({ new_cell.i, new_cell.j + 1, t });
            break;
        }
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

void SpaceTimeSearch::SetRadius(float new_radius)
{
    if (new_radius < 0 || new_radius > 0.5) return;
    radius_ = new_radius;
}

bool SpaceTimeSearch::IsDiagonalCollision(SpaceTimeCell from, SpaceTimeCell to) const
{
    Move reverse_move = SpaceTimeSearch::GetMove(to, from);
    Move left_move = SpaceTimeSearch::GetMove(to, { from.i, to.j });
    Move down_move = SpaceTimeSearch::GetMove(to, { to.i, from.j });

    // Parallel collision
    if (radius_ > sqrt(2) / 4)
    {
        int delta_j = to.j - from.j;
        auto iterator = reservation_->find({ from.i, from.j - delta_j, to.t });
        if (iterator != reservation_->end() && iterator->move == reverse_move) return true;

        int delta_i = to.i - from.i;
        iterator = reservation_->find({ from.i - delta_i, from.j, to.t });
        if (iterator != reservation_->end() && iterator->move == reverse_move) return true;
    }

    // Close diagonal move
    if (radius_ > 0.1)
    {
        auto iterator = reservation_->find({ from.i, to.j, to.t });
        if (iterator != reservation_->end() && iterator->move == left_move) return true;

        iterator = reservation_->find({ from.i, from.j, to.t });
        if (iterator != reservation_->end() && (
            iterator->move == down_move || iterator->move == left_move)) return true;

        iterator = reservation_->find({ to.i, from.j, to.t });
        if (iterator != reservation_->end() && iterator->move == down_move) return true;
    }

    return false;
}

bool SpaceTimeSearch::IsNondiagonalCollision(SpaceTimeCell from, SpaceTimeCell to) const
{
    int perp_delta_i = 1 - abs(to.i - from.i);
    int perp_delta_j = 1 - abs(to.j - from.j);

    // Close diagonal move
    if (options_.allowdiagonal && radius_ > 0.1)
    {
        Move up_left_move = SpaceTimeSearch::GetMove(to, { from.i + perp_delta_i, from.j + perp_delta_j });
        Move down_left_move = SpaceTimeSearch::GetMove(to, { from.i - perp_delta_i, from.j - perp_delta_j });

        auto iterator = reservation_->find({ from.i, from.j, to.t });
        if (iterator != reservation_->end() && (
            iterator->move == up_left_move || iterator->move == down_left_move)) return true;

        iterator = reservation_->find({ from.i + perp_delta_i, from.j + perp_delta_j, to.t });
        if (iterator != reservation_->end() && iterator->move == up_left_move) return true;

        iterator = reservation_->find({ from.i - perp_delta_i, from.j - perp_delta_j, to.t });
        if (iterator != reservation_->end() && iterator->move == down_left_move) return true;
    }

    // Close nondiagonal move
    if (radius_ > 0.25)
    {
        Move up_move = SpaceTimeSearch::GetMove(to, { to.i + perp_delta_i, to.j + perp_delta_j });
        Move down_move = SpaceTimeSearch::GetMove(to, { to.i - perp_delta_i, to.j - perp_delta_j });

        auto iterator = reservation_->find({ to.i + perp_delta_i, to.j + perp_delta_j, to.t });
        if (iterator != reservation_->end() && iterator->move == up_move) return true;

        auto iterator = reservation_->find({ to.i - perp_delta_i, to.j - perp_delta_j, to.t });
        if (iterator != reservation_->end() && iterator->move == down_move) return true;
    }

    return false;
}
