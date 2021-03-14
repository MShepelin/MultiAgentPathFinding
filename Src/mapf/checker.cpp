#include "checker.h"
#include "astar3d.h"

void Checker::SetConfiguration(const Map* map, const Config& config)
{
    // Set map
    map_ = map;

    // Initialise evironment parameters
    options_ = EnvironmentOptions(
        config.SearchParams[CN_SP_AS],
        config.SearchParams[CN_SP_AD],
        config.SearchParams[CN_SP_CC],
        config.SearchParams[CN_SP_MT]
    );
}

bool Checker::CheckNaive(
    const std::unordered_set<SpaceTimeCell>* reservation, 
    std::ostream* log_stream)
{
    bool result = true;

    for (auto iterator : *reservation)
    {
        if (iterator.move == Move::Spawn) continue;

        SpaceTimeCell target = iterator;
        SpaceTimeCell parent = iterator;

        // Set parent
        parent.t -= 1;

        unsigned char move = static_cast<unsigned char>(target.move);
        if (move < 3)
        {
            parent.i -= 1;
        }
        else if (move > 5)
        {
            parent.i += 1;
        }

        if (move % 3 == 2)
        {
            parent.j += 1;
        }
        else if (move % 3 == 0)
        {
            parent.j -= 1;
        }

        if (reservation->find(parent) == reservation->end())
        {
            *log_stream << "The move of cell (" <<
                target.i << ", " << target.j << ", " << target.t << 
                ") is incorrect: no parential node found\n";

            result = false;
            continue;
        }

        SpaceTimeCell swapping_target = parent;
        swapping_target.t += 1;
        SpaceTimeCell swapping_parent = target;
        swapping_parent.t -= 1;

        auto find_target = reservation->find(swapping_target);

        if (!(swapping_target == target) &&
            find_target != reservation->end() && 
            find_target->move == SpaceTimeSearch::GetMove(swapping_parent, swapping_target) &&
            reservation->find(swapping_parent) != reservation->end())
        {
            *log_stream << "The move of cell (" <<
                target.i << ", " << target.j << ", " << target.t <<
                ") is incorrect: swapping conflict\n";
            result = false;
            continue;
        }
    }

    if (result) *log_stream << "NO ";
    *log_stream << "CONFLICTS\n";

    return result;
}
