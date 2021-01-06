#include "ratio_finder.h"

#include <set>
#include <map>
#include <vector>
#include <string>

#include "query_helper.h"

void FindRootsAndBaseRatios(
    std::vector<std::string> const &nodes, QueryHelper const &get_connections,
    std::map<std::string, ConnectionAndCost> &node_roots_and_ratios) {
  std::set<std::string> unused_nodes(nodes.begin(), nodes.end());

  while (!unused_nodes.empty()) {
    const auto curr_root = *(unused_nodes.begin());
    unused_nodes.erase(unused_nodes.begin());

    node_roots_and_ratios[curr_root] = {curr_root, 1.0};

    // Look for all connections to the current root node. Add them to the output
    // map.
    std::deque<std::pair<std::string, double>> queue;
    queue.push_back({curr_root, 1.0});
    while (!queue.empty()) {
      const auto node = queue.front();
      queue.pop_front();

      const auto nearby_connections = get_connections(node.first);
      for (auto conn : nearby_connections) {
        auto unused_node_iter = unused_nodes.find(conn.first);

        if (unused_node_iter == unused_nodes.end()) {
          continue;
        }

        // Put current connection in the result, pointing back to the subgraph
        // root.
        node_roots_and_ratios[conn.first] = {curr_root,
                                             node.second * conn.second};
        queue.push_back({conn.first, node.second * conn.second});

        // Erase from global "unused" list.
        unused_nodes.erase(unused_node_iter);
      }  // nearby_connections loop
    }    // subgraph loop
  }      // outer graph (forest) loop
}
