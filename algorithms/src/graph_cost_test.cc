#include "graph_cost.h"

#include <cassert>
#include <map>
#include <string>
#include <vector>
#include <gtest/gtest.h>

#include "simple_graph.h"

static const V_GraphEdge kBasicGraphEdges = {
    {"A", "B", 2.0}, {"B", "C", 4.0}, {"X", "K", 5.0}, {"Z", "G", 3.0},
};

TEST(SimpleGraph, easy_connections) {
  SimpleGraph graph(kBasicGraphEdges);

  QueryHelper get_connections = [&](std::string start) -> V_ConnectionAndCost {
    return graph.getConnections(start);
  };

  EXPECT_EQ(2.0, findCost(get_connections, "A", "B"));
  EXPECT_EQ(8.0, findCost(get_connections, "A", "C"));
  EXPECT_EQ(-1, findCost(get_connections, "A", "K"));
  EXPECT_EQ(0.125, findCost(get_connections, "C", "A"));
}
