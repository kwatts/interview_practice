#include <string>
#include <vector>
#include <map>

#include "query_helper.h"

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
  SimpleGraph(const V_GraphEdge& edges);

  V_ConnectionAndCost getConnections(std::string start) const;

  std::vector<std::string> getNodes() const;

 private:
  std::map<std::string, V_ConnectionAndCost> connections_;
};
