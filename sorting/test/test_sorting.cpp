/*
 * Unit tests for heap and radix sort
 */

#include "heap_sort.hpp"
#include "radix_sort.hpp"
#include "testing_common.hpp"

#include <algorithm>  // std::sort, std::is_sorted
#include <random>     // generate random input data

#include <iostream>

#include <time.h>

#define BOOST_TEST_MODULE SortingPractice
#include <boost/test/unit_test.hpp>

// Functions can't be passed directly as template parameters
template <typename IteratorT>
struct HeapSortFunctorWrapper {
  void operator()(IteratorT start, IteratorT end) const {
    return sorting_practice::heapSort(start, end);
  }
};

template <typename IteratorT>
struct RadixSortFunctorWrapper {
  void operator()(IteratorT start, IteratorT end) const {
    return sorting_practice::radixSort(start, end);
  }
};

/*
 * Test both radix and heap sort for speed and correctness
 */
BOOST_AUTO_TEST_CASE(SortingPerformance) {
  static const size_t LARGE_VEC = 1000000;

  typedef uint32_t UnsignedIntT;

  std::vector<UnsignedIntT> elements;
  sorting_practice::testing::createRandomVector<UnsignedIntT>(elements, 10000,
                                                              LARGE_VEC);

  UnsignedIntT* start = &elements.data()[0];
  UnsignedIntT* end = start + elements.size();

  // radix
  BOOST_REQUIRE(sorting_practice::testing::testPerformance(
      start, end, RadixSortFunctorWrapper<UnsignedIntT*>(), "radix_sort"));

  // heap
  BOOST_REQUIRE(sorting_practice::testing::testPerformance(
      start, end, HeapSortFunctorWrapper<UnsignedIntT*>(), "heap_sort"));
}

/*
 * Radix sorting should support custom types, with unsigned integer keys
 */
struct MyStruct {
  uint32_t key;
  std::string dummy_value;
};

namespace sorting_practice {

template <>
struct RadixValueTraits<MyStruct> {
  typedef uint32_t RadixKeyT;

  static RadixKeyT getKey(MyStruct const& v) { return v.key; }
};

}  // sorting_practice

// Define this, so std::sort works
bool operator<(const MyStruct& k1, const MyStruct& k2) {
  return k1.key < k2.key;
}

// Verify radix sort works for custom type
BOOST_AUTO_TEST_CASE(RadixSortingCustomType) {
  static const size_t LARGE_VEC = 1000000;

  typedef uint32_t UnsignedIntT;

  std::vector<UnsignedIntT> elements;
  sorting_practice::testing::createRandomVector<UnsignedIntT>(elements, 10000,
                                                              LARGE_VEC);

  std::vector<MyStruct> struct_elements;
  struct_elements.reserve(elements.size());
  for (size_t i = 0; i < elements.size(); ++i) {
    MyStruct mys;
    mys.key = elements[i];

    struct_elements.push_back(mys);
  }

  MyStruct* start = &struct_elements.data()[0];
  MyStruct* end = start + struct_elements.size();

  // radix
  BOOST_REQUIRE(sorting_practice::testing::testPerformance(
      start, end, RadixSortFunctorWrapper<MyStruct*>(), "radix_sort"));
}
