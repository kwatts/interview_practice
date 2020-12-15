/*
 * Implements least-significant-digit radix sort
 *
 * Unsigned integer keys only.
 *
 * (Uses STL vector for "bucket" types)
 */

#ifndef RADIX_SORT_HPP_
#define RADIX_SORT_HPP_

#include "common.hpp"

#include <cassert>
#include <type_traits>

#include <vector>  // bucket type

#if __GNUC__ < 4
#error "GCC version is not at least version 4!"
#endif

#if __GNUC__ == 4 && __GNUC_MINOR__ < 8
#error "GCC version must be >=4.8! "
#endif

namespace sorting_practice {

// Radix sort operates on unsigned integer "keys"
// but the sorted values can be a struct which contains a key
// Default is to use Key = Value
template <typename ValueT>
struct RadixValueTraits {
  typedef ValueT RadixKeyT;  // unsigned integer type

  inline static RadixKeyT getKey(ValueT const &v) { return v; }
};

namespace impl {

// Use 4 bit buckets (16)
template <typename RadixKeyT>
inline std::size_t getBucket4Bit(RadixKeyT const &key,
                                 std::size_t bucket_shift) {
  static const std::size_t MASK = 15;

  std::size_t rv = (key & (MASK << bucket_shift)) >> bucket_shift;
  assert(rv >= 0 && rv < 16);
  return rv;
}

}  // impl

/*
 * Sort a range of RadixValues, which contain a key
 */
template <
    class IteratorT,
    class = typename std::enable_if<
        IsUnsignedIntegral<typename RadixValueTraits<
            typename IteratorTraits<IteratorT>::ValueType>::RadixKeyT>::value,
        int>::type>
void radixSort(IteratorT start, IteratorT end) {
  typedef typename IteratorTraits<IteratorT>::ValueType ValueT;
  typedef typename RadixValueTraits<ValueT>::RadixKeyT RadixKeyT;

  static_assert(IsUnsignedIntegral<RadixKeyT>::value,
                "Radix key must be unsigned integer type! Specialize template "
                "for \"IsUnsignedIntegral\"");

  std::size_t key_size = sizeof(RadixKeyT);
  std::size_t num_sorts = key_size * 2;  // We use 4bit buckets

  // Use "buckets" to store values
  typedef std::vector<ValueT> BucketT;
  std::vector<BucketT> buckets(16);

  // Least-significant-bucket sorting, each bucket is 4bits
  for (std::size_t i = 0; i < num_sorts; ++i) {
    std::size_t bucket_shift = i * 4;  // shift in bits

    // Assign all elements to buckets
    for (IteratorT it = start; it != end; ++it) {
      RadixKeyT key = RadixValueTraits<ValueT>::getKey(*it);

      buckets[impl::getBucket4Bit(key, bucket_shift)].push_back(*it);
    }

    // Load all elements from buckets back to range
    IteratorT range_start = start;
    for (std::size_t i = 0; i < buckets.size(); ++i) {
      BucketT &my_bucket = buckets[i];
      for (std::size_t j = 0; j < my_bucket.size(); ++j) {
        *(range_start++) = my_bucket[j];
      }
      my_bucket.clear();
    }
    assert(range_start == end);  // we've loaded all elements
  }
}

}  // sorting_practice

#endif /* RADIX_SORT_HPP_ */
