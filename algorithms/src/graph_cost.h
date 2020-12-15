#include <functional>
#include <string>
#include <vector>

using ConnectionAndCost = std::pair<std::string, double>;
using V_ConnectionAndCost = std::vector<ConnectionAndCost>;

// Accessor for costs of nearby nodes.
using QueryHelper = std::function<V_ConnectionAndCost(std::string)>;

// Find cost of a query in a graph.
double findCost(QueryHelper const& get_connections, std::string start,
                std::string end);
