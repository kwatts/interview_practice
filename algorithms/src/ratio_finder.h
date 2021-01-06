#ifndef INTERVIEW_PRACTICE_RATIO_FINDER_H_
#define INTERVIEW_PRACTICE_RATIO_FINDER_H_
#include <algorithm>
#include <deque>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "query_helper.h"

// For a given graph (nodes and edges), separate the graph into a multi-graph
// with different ratios.
//
// Return value: map for each edge, with root node and ratio to the root node.
void FindRootsAndBaseRatios(
    std::vector<std::string> const &nodes, QueryHelper const &get_connections,
    std::map<std::string, ConnectionAndCost> &node_roots_and_ratios);

#endif  // INTERVIEW_PRACTICE_RATIO_FINDER_H_
