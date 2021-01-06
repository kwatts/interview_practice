#include "ratio_finder.h"

#include <gtest/gtest.h>

#include "simple_graph.h"

static const V_GraphEdge kBasicGraphEdges = {
    {"A", "B", 2.0}, {"B", "C", 4.0}, {"X", "K", 5.0}, {"Z", "G", 3.0},
};

TEST(RatioFinder, singleRootNode) {
  SimpleGraph graph(kBasicGraphEdges);

  QueryHelper get_connections = [&](std::string start) -> V_ConnectionAndCost {
    return graph.getConnections(start);
  };

  std::map<std::string, ConnectionAndCost> result;

  FindRootsAndBaseRatios(graph.getNodes(), get_connections, result);

  EXPECT_EQ(result.size(), 7);
}
