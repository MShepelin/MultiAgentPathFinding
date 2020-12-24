#ifndef NODE_H
#define NODE_H

//That's the data structure for storing a single search node.
//You MUST store all the intermediate computations occuring during the search
//incapsulated to Nodes (so NO separate arrays of g-values etc.)

struct Node
{
    int     i, j; //grid cell coordinates
    double  g = -1, H = -1; //g- and h-values of the search node, if H == -1 Node is considered to be in a "close" list
    Node    *parent = nullptr; //backpointer to the predecessor node (e.g. the node which g-value was used to set the g-velue of the current node)
    long long key;

    bool operator>(Node& nodeToCompare) const
    {
        return g + H > nodeToCompare.g + nodeToCompare.H;
    }
};
#endif
