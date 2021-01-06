#include <string>
#include <vector>
#include <functional>

#include "query_helper.h"

// Find cost of a query in a graph.
double findCost(QueryHelper const& get_connections, std::string start,
                std::string end);
