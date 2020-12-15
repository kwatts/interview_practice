#include "graph_cost.h"

#include <cassert>
#include <map>
#include <string>
#include <vector>

#include "testing_util.h"

// Each graph edge gives the cost from left->right.
// Cost from right->left is the multiplicative inverse (1.0 / COST).
struct GraphEdge {
  std::string left;
  std::string right;
  double cost;
};

using V_GraphEdge = std::vector<GraphEdge>;

// Simple graph for testing.
class SimpleGraph {
 public:
  SimpleGraph(const V_GraphEdge& edges) {
    // std::map operator[] will default initialize if key is not found.
    for (const auto& conn : edges) {
      connections_[conn.left].push_back({conn.right, conn.cost});
      connections_[conn.right].push_back({conn.left, 1.0 / conn.cost});
    }
  }

  V_ConnectionAndCost getConnections(std::string start) const {
    auto node_connections = connections_.find(start);
    if (node_connections == connections_.end()) {
      return {};
    }

    return node_connections->second;
  }

 private:
  std::map<std::string, V_ConnectionAndCost> connections_;
};

static const V_GraphEdge kBasicGraphEdges = {
    {"A", "B", 2.0}, {"B", "C", 4.0}, {"X", "K", 5.0}, {"Z", "G", 3.0},
};

int main(int argc, char** argv) {
  SimpleGraph graph(kBasicGraphEdges);

  QueryHelper get_connections = [&](std::string start) -> V_ConnectionAndCost {
    return graph.getConnections(start);
  };

  CHECK(2.0 == findCost(get_connections, "A", "B"), "");
  CHECK(8.0 == findCost(get_connections, "A", "C"), "");
  CHECK(-1 == findCost(get_connections, "A", "K"), "");
  CHECK(0.125 == findCost(get_connections, "C", "A"), "");
}
