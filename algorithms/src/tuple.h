#include "type_traits.h"

namespace tuple {

// Options for setting this up:
// - Recursively (base case, etc).
template<typename ...Args>
struct Tuple {
  
  template<typename ...CArgs>
  explicit Tuple(CArgs ...cargs);

  // typedef's for types
  
};

template<int index, typename Tuple<Args...>>
auto get(Tuple<Args...> const &t);

}  // namespace tuple
