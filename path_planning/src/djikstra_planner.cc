#include "djikstra_planner.hh"

#include <algorithm>
#include <cmath>
#include <queue>
#include <utility>

#include "map_2d.hh"

namespace {

/** Returns valid neighbors in up/down and left/right directions. */
std::vector<Cell> getNeighbors(Map2D<float> const &costMap, Cell start) {
  std::vector<Cell> result;
  static const int dr[] = {-1, 1, 0, 0};
  static const int dc[] = {0, 0, -1, 1};
  for (int i = 0; i < 4; ++i) {
    int row = start.row + dr[i];
    int col = start.col + dc[i];

    if (row < 0 || row >= costMap.getHeight()) {
      continue;
    }
    if (col < 0 || col >= costMap.getWidth()) {
      continue;
    }

    result.push_back(Cell(row, col));
  }
  return result;
}

struct PathCostToCell {
  double cost;
  Cell cell;

  PathCostToCell(double _cost, Cell _cell) : cost(_cost), cell(_cell) {}
};

// std::greater
bool operator>(PathCostToCell const &p1, PathCostToCell const &p2) {
  return p1.cost > p2.cost;
}

}  // namespace

double computePath(Cell start, Cell end, Map2D<float> const &costMap,
                   std::vector<Cell> &path) {
  Map2D<impl::CellCostAndParent> explored_map =
      Map2D<impl::CellCostAndParent>(costMap.getWidth(), costMap.getHeight());

  // Priority queue ordered by minimum cost ("first").
  // Note that the same value can be added to the queue with multiple costs.
  // Cells with
  std::priority_queue<PathCostToCell, std::deque<PathCostToCell>

                      , std::greater<PathCostToCell> > queue;

  double start_cost = costMap.getCost(start.row, start.col);
  queue.push(PathCostToCell(start_cost, start));
  explored_map.getCost(start.row, start.col).cost = start_cost;

  while (!queue.empty()) {
    const PathCostToCell current = queue.top();
    queue.pop();

    // Check if we've already processed this cell with a lower cost.
    if (current.cost >
        explored_map.getCost(current.cell.row, current.cell.col).cost) {
      continue;
    }

    const auto &neighbors = getNeighbors(costMap, current.cell);
    for (const Cell n : neighbors) {
      if (std::isinf(costMap.getCost(n.row, n.col))) {
        continue;
      }

      // Cost to move one cell.
      static const double TRAVEL_COST = 1.0;
      double neighbor_cost =
          current.cost + costMap.getCost(n.row, n.col) + TRAVEL_COST;

      // See if we're already reached this cell from another direction.
      // Ex: parent, or lower-cost path.
      if (neighbor_cost > explored_map.getCost(n.row, n.col).cost) {
        continue;
      }

      queue.push(PathCostToCell(neighbor_cost, n));
      explored_map.getCost(n.row, n.col).cost = neighbor_cost;
      explored_map.getCost(n.row, n.col).parent = current.cell;
    }
  }

  return findPathFromExploration(start, end, explored_map, path);
}

namespace impl {

double findPathFromExploration(Cell start, Cell end,
                               Map2D<CellCostAndParent> const &explored_map,
                               std::vector<Cell> &path) {
  // Work backwards from end, using the parent cell in explored_map.
  Cell current = end;

  while (current != start) {
    double curr_cost = explored_map.getCost(current.row, current.col).cost;
    if (std::isinf(curr_cost) ||
        !explored_map.getCost(current.row, current.col).parent) {
      path.clear();
      return std::numeric_limits<double>::infinity();
    }

    path.push_back(current);
    current = *(explored_map.getCost(current.row, current.col).parent);
  }

  path.push_back(start);

  std::reverse(path.begin(), path.end());

  return explored_map.getCost(end.row, end.col).cost;
}

}  // namespace impl
