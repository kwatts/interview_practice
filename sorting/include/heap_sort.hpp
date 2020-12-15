/*
 * Implements heap sort for arbitrary types
 */

#ifndef SORTING_PRACTICE_HEAP_SORT
#define SORTING_PRACTICE_HEAP_SORT

#include "common.hpp"

namespace sorting_practice
{

/*
 * Implements heap sort between [start, end)
 */
template<typename IteratorT, typename LessT = sorting_practice::Less<typename IteratorTraits<IteratorT>::ValueType> >
void heapSort(IteratorT start, IteratorT end, LessT const &cmp = LessT());

namespace impl
{

template<typename DiffT>
inline DiffT leftIdx(DiffT idx)
{
    return 2 * idx;
}

template<typename DiffT>
inline DiffT rightIdx(DiffT idx)
{
    return 2 * idx + 1;
}

// Iteratively go through the array and enforce heap property: parents >= children
template<typename IteratorT, typename LessT>
void maxHeapify(IteratorT start,
        typename IteratorTraits<IteratorT>::DifferenceType idx,
        typename IteratorTraits<IteratorT>::DifferenceType max_len,
        LessT const &cmp)
{
    while (idx < max_len - 1)
    {
        typename IteratorTraits<IteratorT>::DifferenceType left_idx  = leftIdx(idx);
        typename IteratorTraits<IteratorT>::DifferenceType right_idx = rightIdx(idx);

        if (left_idx >= max_len)
        {
            return;
        }

        IteratorT parent = start + idx;
        IteratorT left = start + left_idx;

        // Case where right > len, not left
        bool left_greater = cmp(*(parent), *(left));

        if (right_idx >= max_len)
        {
            if (left_greater)
            {
                sorting_practice::iterSwap(parent, left);
            }
            return;
        }

        IteratorT right = start + right_idx;
        bool right_greater = cmp(*(parent), *(right));

        if (right_greater || left_greater)
        {
            if (cmp(*(left), *(right)))
            {
                sorting_practice::iterSwap(parent, right);
                idx = right_idx;
                continue;
            }
            else
            {
                sorting_practice::iterSwap(parent, left);
                idx = left_idx;
                continue;
            }
        }

        return; // heap property satisfied
    }
}

// Create a heap in linear time from end -> start of array
template<typename IteratorT, typename LessT>
void makeHeap(IteratorT start, IteratorT end, LessT const &cmp)
{
    typename IteratorTraits<IteratorT>::DifferenceType len = end - start;
    typename IteratorTraits<IteratorT>::DifferenceType idx = len / 2 + 1;
    while (idx > 0)
    {
        maxHeapify(start, idx, len, cmp);
        --idx;
    }
    maxHeapify(start, 0, len, cmp);
}

} // impl

template<typename IteratorT, typename LessT = sorting_practice::Less<typename IteratorTraits<IteratorT>::ValueType> >
void heapSort(IteratorT start, IteratorT end, LessT const &cmp = LessT())
{
    typename IteratorTraits<IteratorT>::DifferenceType total_len = end - start;
    typename IteratorTraits<IteratorT>::DifferenceType heap_size = total_len;

    impl::makeHeap(start, end, cmp);

    while (heap_size > 0)
    {
        sorting_practice::iterSwap(start, start + (heap_size - 1));
        heap_size -= 1;
        impl::maxHeapify(start, 0, heap_size, cmp);
    }
}

} // sorting_practice

#endif // SORTING_PRACTICE_HEAP_SORT
