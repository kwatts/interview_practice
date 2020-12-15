#include "graph_cost.h"

#include <deque>
#include <functional>
#include <set>
#include <string>
#include <vector>

// Find cost of a query in a graph.
double findCost(QueryHelper const& get_connections, std::string start,
                std::string end) {
  const auto first_nodes = get_connections(start);

  // Nodes that we need to search.
  // intersection(active_nodes, visited) = NULL
  std::deque<std::pair<std::string, double>> active_nodes(first_nodes.begin(),
                                                          first_nodes.end());

  // Visited: All nodes which have been consumed from active_nodes.
  std::set<std::string> visited;

  while (!active_nodes.empty()) {
    const std::pair<std::string, double> conn = active_nodes.front();
    active_nodes.pop_front();

    visited.insert(conn.first);

    if (conn.first == end) {
      return conn.second;
    }

    const auto nearby_connections = get_connections(conn.first);
    for (auto nearby : nearby_connections) {
      if (visited.find(nearby.first) != visited.end()) {
        continue;
      }

      // Accumulate cost from current node.
      nearby.second *= conn.second;

      active_nodes.push_back(nearby);
    }
  }

  return -1;
}
