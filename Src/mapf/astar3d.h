#pragma once

#include <cassert>
#include "search.h"
#include <unordered_set>

class SpaceTimeSearch : public SingleSearch<SpaceTimeCell>
{
protected:
    SingleSearch<GridCell>* heuristic_;
    std::unordered_set<SpaceTimeCell>* reservation_;
    static const FTYPE cost; // set FTYPE as a type of single search

protected:
    virtual void SetHeuristic(NodeType& node_to_edit) override;

    virtual void ExpandNode(NodeType* node_to_expand) override;

    virtual FTYPE GetMoveCost(SpaceTimeCell from, SpaceTimeCell to) const override;

    virtual const NodeType* CellIsReached(SpaceTimeCell cell) const override;

public:
    virtual bool NodeReachedCell(NodeType* node, SpaceTimeCell cell, FTYPE depth) const override;

    virtual void BuildCompactPath() override;

    SpaceTimeSearch() = delete;

    SpaceTimeSearch(SingleSearch<GridCell>* heuristic, std::unordered_set<SpaceTimeCell>* reservation);

    void WritePath() const;

    static Move GetMove(SpaceTimeCell from, SpaceTimeCell to);
};
