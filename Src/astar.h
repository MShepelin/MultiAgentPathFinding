#pragma once

#include "search.h"

class GridSingleSearch : public SingleSearch<GridCell>
{
public:
    static const FTYPE ch;
    static const FTYPE cd;

protected:
    virtual void SetHeuristic(NodeType& node_to_edit) override;

    virtual void ExpandNode(NodeType* node_to_expand) override;

    virtual FTYPE GetMoveCost(GridCell from, GridCell to) const override;

public:
    virtual void BuildCompactPath() override;
};
