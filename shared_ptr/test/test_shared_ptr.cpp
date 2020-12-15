
#include "shared_ptr.hpp"

#include <cstddef>  // std::size_t
#include <stdint.h>

#include "pool_allocator.hpp"

#define BOOST_TEST_MODULE SharedPtrPractice
#include <boost/test/unit_test.hpp>

// Empty pointer
BOOST_AUTO_TEST_CASE(SharedPtrStandardAllocEmpty)
{
    practice::SharedPtr<int> none;

    BOOST_CHECK(none.count() == 0);
    BOOST_CHECK(none.get() == nullptr);
}

practice::SharedPtr<int> makePtr(int value)
{
    practice::SharedPtr<int> rv(new int);
    *rv = value;
    return rv;
}

// Allocate and share values
BOOST_AUTO_TEST_CASE(SharedPtrStandardAlloc)
{
    static const int VALUE = 5;

    practice::SharedPtr<int> ptr(new int);
    *ptr = VALUE; // assign value

    BOOST_CHECK(ptr.count() == 1);
    BOOST_CHECK(ptr.get() != nullptr);
    BOOST_CHECK(VALUE == *ptr);

    ptr = ptr; // self assignment
    BOOST_CHECK(ptr.count() == 1);
    BOOST_CHECK(ptr.get() != nullptr);
    BOOST_CHECK(VALUE == *ptr);

    practice::SharedPtr<int> ptr_assign;
    ptr_assign = ptr;
    BOOST_CHECK(ptr.count() == 2);
    ptr_assign.reset();
    BOOST_CHECK(ptr.count() == 1);

    {
        practice::SharedPtr<int> ptr2(ptr);
        BOOST_CHECK(ptr.count()  == 2);
        BOOST_CHECK(ptr2.count() == 2);

        BOOST_CHECK(ptr.get() == ptr2.get());
    }

    BOOST_CHECK(ptr.count() == 1);
    BOOST_CHECK(ptr.get() != nullptr);
    BOOST_CHECK(VALUE == *ptr);

    // Move constructor
    practice::SharedPtr<int> ptr3 = makePtr(VALUE);
    BOOST_CHECK(ptr3.count() == 1);
    BOOST_CHECK(ptr3.get() != nullptr);
    BOOST_CHECK(VALUE == *ptr3);

    ptr3.reset();
    BOOST_CHECK(ptr3.count() == 0);
    BOOST_CHECK(!ptr3);
    BOOST_CHECK(ptr3.get() == nullptr);

    // Move assign
    practice::SharedPtr<int> ptr4(new int);
    *ptr4 = VALUE;
    ptr3 = practice::move(ptr4);
    BOOST_CHECK(ptr4.count() == 0);
    BOOST_CHECK(ptr3.count() == 1);
    BOOST_CHECK(VALUE == *ptr3);
}

struct Simple
{
    int a, b, c;
};

// Non integer types and assigning data
BOOST_AUTO_TEST_CASE(SharedPtrStruct)
{
    static const int VALUE = 5;

    Simple *raw_p = new Simple;

    practice::SharedPtr<Simple> ptr(raw_p);

    BOOST_CHECK(ptr.count() == 1);
    BOOST_CHECK(ptr.get() == raw_p);

    ptr->a = VALUE;
    BOOST_CHECK(raw_p->a == VALUE);

    // swap
    practice::SharedPtr<Simple> ptr2;
    ptr.swap(ptr2);
    BOOST_CHECK(ptr.count()  == 0);
    BOOST_CHECK(ptr2.count() == 1);
}

// Test of the pool allocator itself
BOOST_AUTO_TEST_CASE(TestPoolAllocator)
{
    typedef practice::PoolAllocator<int32_t> PoolAllocatorInt32;

    practice::MemoryPool pool(1, sizeof(int32_t));

    PoolAllocatorInt32 alloc(&pool);

    BOOST_CHECK(alloc.size() == 0);
    BOOST_CHECK(alloc.capacity() == 1);

    int32_t *ptr = nullptr;
    BOOST_CHECK_NO_THROW(ptr = alloc.allocate(sizeof(int32_t)));
    BOOST_CHECK(ptr != nullptr);

    // See what happens for full alloc
    int32_t *ptr2 = nullptr;
    BOOST_CHECK_THROW(ptr2 = alloc.allocate(sizeof(int32_t)), practice::PoolAllocationException);
    BOOST_CHECK(ptr2 == nullptr); // unused-but-set-variable

    // Now we should be able to alloc
    BOOST_CHECK_NO_THROW(alloc.deallocate(ptr, sizeof(int32_t)));
    BOOST_CHECK(alloc.size() == 0);
}

/*
 * Use our awesome PoolAllocator to track the memory we're actually using (not just pointer state)
 *
 * Note: This won't track that we're allocating the count properly
 */
BOOST_AUTO_TEST_CASE(TestSharedPtrAllocator)
{
    typedef practice::PoolAllocator<Simple>                  PoolAllocatorSimple;
    typedef practice::SharedPtr<Simple, PoolAllocatorSimple> SharedPtrSimple;

    practice::MemoryPool pool(10, sizeof(Simple));

    PoolAllocatorSimple alloc(&pool);

    // Null object
    SharedPtrSimple ptr(nullptr, alloc);

    BOOST_CHECK(alloc.size() == 0);
    BOOST_CHECK(alloc.capacity() == 10);

    // Valid object, create and destroy
    SharedPtrSimple ptr2(alloc.allocate(sizeof(Simple)), alloc);
    BOOST_CHECK(alloc.size() == 1);
    ptr2.reset();
    BOOST_CHECK(alloc.size() == 0);

    // Shared objects
    // Allocated memory stays constant, but the count increases
    SharedPtrSimple ptr3(alloc.allocate(sizeof(Simple)), alloc);
    BOOST_CHECK(alloc.size() == 1);
    {
        SharedPtrSimple ptr3_share(ptr3);
        BOOST_CHECK(ptr3_share.count() == 2);
        BOOST_CHECK(alloc.size() == 1);
        {
            SharedPtrSimple ptr3_share2(ptr3);
            BOOST_CHECK(alloc.size() == 1);
            BOOST_CHECK(ptr3_share2.count() == 3);
        }
        BOOST_CHECK(ptr3_share.count() == 2);
        BOOST_CHECK(alloc.size() == 1);
    }
    BOOST_CHECK(alloc.size() == 1);
}
