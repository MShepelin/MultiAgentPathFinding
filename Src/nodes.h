#pragma once

#include <functional>
#include "gl_const.h"

// Hash definition

// The difinitions of MAKE_HASHABLE and hash_combine are borrowed from:
// https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x

#define MAKE_HASHABLE(T, ...) \
    namespace std {\
        template<> struct hash<T> {\
            size_t operator()(const T &type) const {\
                size_t ret = 0;\
                hash_combine(ret, __VA_ARGS__);\
                return ret;\
            }\
        };\
    }

inline void hash_combine(std::size_t& seed) {};

template <typename T, typename... Rest>
inline void hash_combine(std::size_t& seed, const T& v, Rest... rest) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    hash_combine(seed, rest...);
}

// Nodes definition

// Node type for using with heap
template<typename CellType>
struct Node
{
    CellType cell;
    // If H == -1 the node is in a "close" list, else it's in an "open" list
    // If g < 0 the node is currently unreachable, else g = current min distance to the node
    FTYPE  g, h;
    size_t heap_index;
    Node<CellType> *parent;

    Node(CellType init_cell)
    {
        cell = init_cell;
        g = 0;
        h = -1;
        parent = nullptr;
    }

    Node(CellType init_cell, FTYPE init_g, FTYPE init_h = -1)
    {
        cell = init_cell;
        g = init_g;
        h = init_h;
        parent = nullptr;
    }

    Node()
    {
        cell = CellType();
        g = -1;
        h = -1;
        parent = nullptr;
    }
};

struct GridCell
{
    int i, j;

    bool operator==(const GridCell& other) const;
};

struct SpaceTimeCell
{
    int i, j, t;

    // Move description
    unsigned char wait : 1;
    unsigned char diagonal : 1;
    unsigned char up : 1;
    unsigned char left : 1;
    
    // @note wait move is described as wait = true, and everything else false

    bool operator==(const SpaceTimeCell& other) const;
};

MAKE_HASHABLE(GridCell, type.i, type.j);
MAKE_HASHABLE(SpaceTimeCell, type.i, type.j, type.t);
