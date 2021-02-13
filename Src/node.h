#pragma once

struct Node
{
    int     i, j; // grid cell coordinates
    double  g = -1, H = -1; // g- and h-values of the search node (negative h-value means that the node is in the "close" list)
    Node    *parent = nullptr; // backpointer to the predecessor node (e.g. the node which g-value was used to set the g-velue of the current node)
    size_t heapIndex;
};
