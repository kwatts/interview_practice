#include "djikstra_planner.hh"

#include <gtest/gtest.h>

#include "map_2d.hh"

static const float INF = std::numeric_limits<float>::infinity();

TEST(findPathFromExploration, oneCellGraph) {
  Map2D<impl::CellCostAndParent> explored_map(/*width=*/1, /*height=*/1);
  explored_map.getCost(0, 0).cost = 0.0;

  std::vector<Cell> path;
  double path_cost = impl::findPathFromExploration(
      /*start=*/Cell(0, 0),
      /*end=*/Cell(0, 0), explored_map, path);

  EXPECT_EQ(0.0, path_cost);
  ASSERT_EQ(1, path.size());
  EXPECT_EQ(0, path[0].row);
  EXPECT_EQ(0, path[0].col);
}

// Path is from [ (0,0), (0,1) ]
TEST(findPathFromExploration, twoCellGraph) {
  Map2D<impl::CellCostAndParent> explored_map(/*width=*/2, /*height=*/1);
  explored_map.getCost(0, 0).cost = 0.0;
  explored_map.getCost(0, 1).cost = 1.0;
  explored_map.getCost(0, 1).parent = Cell(0, 0);

  const Cell start(0, 0);
  const Cell end(0, 1);

  std::vector<Cell> path;
  double path_cost =
      impl::findPathFromExploration(start, end, explored_map, path);

  EXPECT_EQ(1.0, path_cost);
  ASSERT_EQ(2, path.size());
  EXPECT_EQ(start, path[0]);
  EXPECT_EQ(end, path[1]);
}

TEST(computePath, oneCellGraph) {
  Map2D<float> cost_map(/*width=*/1, /*height=*/1);
  cost_map.getCost(0, 0) = 0.0;

  const Cell start(0, 0);

  std::vector<Cell> path;
  double path_cost = computePath(start, start, cost_map, path);

  EXPECT_EQ(0.0, path_cost);
  ASSERT_EQ(1, path.size());
  EXPECT_EQ(start, path[0]);
}

TEST(computePath, twoByTwoGraph) {
  Map2D<float> cost_map(/*width=*/2, /*height=*/2,
                        // Values packed row major.
                        {0.0, INF,  //
                         0.0, 0.0});

  const Cell start(0, 0);
  const Cell end(1, 1);

  std::vector<Cell> path;
  double path_cost = computePath(start, end, cost_map, path);

  EXPECT_EQ(2.0, path_cost);
  ASSERT_EQ(3, path.size());
  EXPECT_EQ(start, path[0]);
  EXPECT_EQ(Cell(1, 0), path[1]);
  EXPECT_EQ(end, path[2]);
}

// All paths feasible, but lowest cost path is indirect.
TEST(computePath, lowCostGraph) {
  Map2D<float> cost_map(/*width=*/5, /*height=*/5,
                        // Values packed row major.
                        {0.0, 100, 100, 100, 0.0,  //
                         0.0, 0.0, 100, 0.0, 0.0,  //
                         100, 0.0, 0.0, 0.0, 100,  //
                         100, 0.0, 0.0, 0.0, 100,  //
                         100, 0.0, 0.0, 0.0, 100});

  const Cell start(0, 0);
  const Cell end(0, 4);

  std::vector<Cell> path;
  double path_cost = computePath(start, end, cost_map, path);

  EXPECT_EQ(8.0, path_cost);
  ASSERT_EQ(9, path.size());
  EXPECT_EQ(start, path[0]);
  EXPECT_EQ(end, path[8]);
}

// Avoid obstacle
TEST(computePath, obstacleGraph) {
  Map2D<float> cost_map(/*width=*/5, /*height=*/5,
                        // Values packed row major.
                        {0.0, 0.0, 0.0, 0.0, 0.0,  //
                         0.0, 0.0, 0.0, 0.0, 0.0,  //
                         0.0, INF, INF, INF, 0.0,  // obstacle!
                         0.0, 0.0, 0.0, 0.0, 0.0,  //
                         0.0, 0.0, 0.0, 0.0, 0.0});

  const Cell start(0, 2);  // top row, middle
  const Cell end(4, 2);    // bottom row, middle

  std::vector<Cell> path;
  double path_cost = computePath(start, end, cost_map, path);

  EXPECT_EQ(8.0, path_cost);
  ASSERT_EQ(9, path.size());
  EXPECT_EQ(start, path[0]);
  EXPECT_EQ(end, path[8]);
}
