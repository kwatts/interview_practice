/*
 * Common includes for sorting functionality
 */

#ifndef SORTING_PRACTICE_COMMON_HPP
#define SORTING_PRACTICE_COMMON_HPP

#include <cstddef>  // std::size_t
#include <stdint.h> // uint8_t, etc

namespace sorting_practice
{

/*
 * Specialize to use custom comparison operators in heap sort
 */
template<typename ValueT>
struct Less
{
    inline bool operator()(ValueT const &v1, ValueT const &v2) const
    {
        return v1 < v2;
    }
};

/*
 * Radix sort key types must be unsigned integer types
 *
 * To define a type as an unsigned integer, call the macro below.
 */
template<typename UnsignedIntT>
struct IsUnsignedIntegral
{
    static constexpr bool value = false;
};

#define DEFINE_UNSIGNED_INT_TYPE(_typname)            \
        template<>                                    \
        struct IsUnsignedIntegral<_typname>           \
        {                                             \
            static constexpr bool value = true;       \
        };                                            \
        template<>                                    \
        struct IsUnsignedIntegral<const _typname>     \
        {                                             \
            static constexpr bool value = true;       \
        };

DEFINE_UNSIGNED_INT_TYPE(uint8_t)
DEFINE_UNSIGNED_INT_TYPE(uint16_t)
DEFINE_UNSIGNED_INT_TYPE(uint32_t)
DEFINE_UNSIGNED_INT_TYPE(uint64_t)

// Supported typedefs
// ValueType
// ReferenceType
// DifferenceType
template<typename IteratorT>
struct IteratorTraits;

template<typename T>
struct IteratorTraits<T*>
{
    typedef T           ValueType;
    typedef T&          ReferenceType;
    typedef std::size_t DifferenceType;
};

template<typename T>
struct IteratorTraits<const T*>
{
    typedef T const     ValueType;
    typedef T const &   ReferenceType;
    typedef std::size_t DifferenceType;
};

template<typename T>
struct RemoveReference
{
    typedef T Type;
};

template<typename T>
struct RemoveReference<T &>
{
    typedef T Type;
};

template<typename T>
struct RemoveReference<T const &>
{
    typedef T Type;
};


template<typename T>
typename sorting_practice::RemoveReference<T>::Type&& move(T &t)
{
    return static_cast<typename sorting_practice::RemoveReference<T>::Type&&>(t);
}

template<typename IteratorT>
void iterSwap(IteratorT i1, IteratorT i2)
{
    auto tmp(sorting_practice::move(*i1));
    *i1 = sorting_practice::move(*i2);
    *i2 = sorting_practice::move(tmp);
}


} // sorting_practice

#endif // SORTING_PRACTICE_COMMON_HPP
