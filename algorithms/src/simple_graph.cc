#include "simple_graph.h"

#include "query_helper.h"

SimpleGraph::SimpleGraph(const V_GraphEdge& edges) {
  // std::map operator[] will default initialize if key is not found.
  for (const auto& conn : edges) {
    connections_[conn.left].push_back({conn.right, conn.cost});
    connections_[conn.right].push_back({conn.left, 1.0 / conn.cost});
  }
}

V_ConnectionAndCost SimpleGraph::getConnections(std::string start) const {
  auto node_connections = connections_.find(start);
  if (node_connections == connections_.end()) {
    return {};
  }

  return node_connections->second;
}

std::vector<std::string> SimpleGraph::getNodes() const {
  std::vector<std::string> result;
  result.reserve(connections_.size());

  for (auto conn : connections_) {
    result.push_back(conn.first);
  }

  return result;
}
