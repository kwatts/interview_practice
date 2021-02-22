#ifndef __DJIKSTRA_PLANNER_HH_
#define __DJIKSTRA_PLANNER_HH_

#include <optional>
#include <vector>

#include "map_2d.hh"

/**
 * Computes min-cost path between start and end. Returns [] if path not found.
 *
 * costMap contains relative cost for each cell. Cost for a path is measured as:
 *
 * pathCost = sum(costMap[r, c] for each cell) + distanceCost
 *
 * All costMap values must be >= 0.0. Infeasible points are +Inf.
 */
double computePath(Cell start, Cell end, Map2D<float> const &costMap,
                   std::vector<Cell> &path);

namespace impl {

/** Store the current cost and the parent (previous node) of the cell. */
struct CellCostAndParent {
  double cost = std::numeric_limits<double>::infinity();
  std::optional<Cell> parent;

  CellCostAndParent() = default;

  CellCostAndParent(CellCostAndParent const &other) = default;
  CellCostAndParent &operator=(CellCostAndParent const &other) = default;
};

/** Finds path from start -> end using exploredMap.
o *
 * Returns empty path and +inf if no path found.
 */
double findPathFromExploration(Cell start, Cell end,
                               Map2D<CellCostAndParent> const &exploredMap,
                               std::vector<Cell> &path);

}  // namespace impl

#endif  // __DJIKSTRA_PLANNER_HH_
