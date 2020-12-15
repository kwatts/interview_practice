// Basic implementations of some STL algorithms.

#include "type_traits.h"

namespace algorithm {

template <typename ValueT>
void swap(ValueT &v1, ValueT &v2) {
  using type_traits::move;
  auto tmp = move(v1);
  v1 = move(v2);
  v2 = move(tmp);
}

template <typename IteratorT>
void reverse(IteratorT begin, IteratorT end) {
  while (begin < end) {
    swap(*begin++, *(--end));
  }
}

template <typename InputIteratorT, typename OutputIteratorT>
void copy(InputIteratorT begin, const InputIteratorT end, OutputIteratorT out) {
  while (begin != end) {
    *out++ = *begin++;
  }
}

} // namespace algorithm
