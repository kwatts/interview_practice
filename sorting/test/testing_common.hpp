/*
 * Utilities for generating and verifying test data for sorting functions
 */

#ifndef TESTING_COMMON_HPP_
#define TESTING_COMMON_HPP_

#include "common.hpp"

#include <string>
#include <algorithm>
#include <iostream>
#include <chrono>

namespace sorting_practice
{

namespace testing
{

template<typename IntT>
void createRandomVector(std::vector<IntT> &vec, IntT max_value, std::size_t max_size)
{
    std::random_device random_dev;
    std::default_random_engine generator(random_dev());
    generator.seed(random_dev());
    std::uniform_int_distribution < IntT > distribution(0, max_value);

    vec.clear();
    vec.reserve(max_size);
    for (std::size_t i = 0; i < max_size; ++i)
    {
        vec.push_back(static_cast<IntT>(distribution(generator)));
    }
}

// Returns time difference in seconds
double timeDiff(clock_t start, clock_t end)
{
    return (static_cast<double>(end) - static_cast<double>(start)) / static_cast<double>(CLOCKS_PER_SEC);
}

/*
 * Returns true if ranges are equivalent (not equal).
 *
 * Tests that sorted output is OK
 *
 * Note: std::mismatch and std::equal use ''equality'' checks
 */
template<typename IteratorT1, typename IteratorT2,
    typename LessT = sorting_practice::Less<typename IteratorTraits<IteratorT1>::ValueType> >
bool rangesEquivalent(IteratorT1 start, IteratorT1 end, IteratorT2 other, LessT const &cmp = LessT())
{
    IteratorT1 it = start;
    IteratorT2 it_other = other;
    for (; it != end; ++it, ++it_other)
    {
        if (cmp(*it, *it_other) || cmp(*it_other, *it))
        {
            return false;
        }
    }

    return true;
}

/*
 * Performance testing
 */
template<typename IteratorT, typename SortFunctionT>
bool testPerformance(IteratorT start, IteratorT end, SortFunctionT sort_fn, std::string const &sort_name)
{
    // Make a copy of the input elements, allows both algorithms to have same input
    typedef typename sorting_practice::IteratorTraits<IteratorT>::ValueType ValueT;
    std::vector<ValueT> range_copy;
    range_copy.resize(end - start);
    std::copy(start, end, range_copy.begin());

    // Custom sort
    clock_t custom_start = clock();
    sort_fn(start, end);
    clock_t custom_stop = clock();

    // Standard sort
    clock_t std_start = clock();
    std::sort(range_copy.begin(), range_copy.end());
    clock_t std_stop = clock();

    bool rv = rangesEquivalent(start, end, range_copy.begin());

    if (!rv)
    {
        std::cerr << "Vector was not sorted!" << std::endl;
    }

    std::cout << "Time for " + sort_name + ": " << (timeDiff(custom_start, custom_stop) * 1.0e3) << "ms" << std::endl;
    std::cout << "Time for std sort:  " << (timeDiff(std_start, std_stop) * 1.0e3) << "ms" << std::endl;
    std::cout << "Number of elements: " << (end - start) << std::endl;

    return rv;
}

} // testing

} // sorting_practice

#endif /* TESTING_COMMON_HPP_ */
