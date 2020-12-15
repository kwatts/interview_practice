#ifndef _PRACTICE_POOL_ALLOCATOR_
#define _PRACTICE_POOL_ALLOCATOR_

#include <memory>

#include <stdio.h>
#include <cstddef>  // std::size_t
#include <stdint.h>
#include <utility>
#include <vector>

#include <iostream>

#include <string.h>

namespace practice
{

class PoolAllocationException : public std::bad_alloc
{

};

class PoolAllocationDesiredSize : public PoolAllocationException
{
public:
    virtual const char *what() const noexcept
    {
        return "Desired size was incorrect!";
    }
};

class PoolAllocationOutOfMemory : public PoolAllocationException
{
public:
    virtual const char *what() const noexcept
    {
        return "Out of memory error!";
    }
};

class PoolAllocationDoubleFree : public PoolAllocationException
{
public:
    virtual const char *what() const noexcept
    {
        return "Attempted double free!";
    }
};

class PoolAllocationSegmentationError : public PoolAllocationException
{
public:
    virtual const char *what() const noexcept
    {
        return "Attempted out of bounds access on pool allocator!";
    }
};

/*
 * Simple pool for fixed size objects
 */
struct MemoryPool
{
    MemoryPool(size_t n, size_t obj_size)
    {
        state.resize(n);
        data.resize(n * obj_size);

        memset(&state[0], 0, n);
    }

    std::vector<uint8_t> state;
    std::vector<uint8_t> data;

    static const uint8_t MEM_AVAILABLE = 0;
    static const uint8_t MEM_USED      = 1;
};

/*
 * PoolAllocator uses MemoryPool to alloc/free objects
 *
 * MemoryPool instance is shared among allocators (shallow-copy)
 */
template<typename T>
class PoolAllocator
{
public:
    PoolAllocator(MemoryPool *pool);
    ~PoolAllocator();

    // shallow copy -> allocators store a pool
    PoolAllocator(PoolAllocator const &other) = default;
    PoolAllocator &operator=(PoolAllocator const &other) = delete;

    size_t size() const;
    size_t capacity() const
    {
        return pool_->state.size();
    }

    T *allocate(size_t desired_sz);
    void deallocate(T* ptr, size_t desired_sz);

    template<class U>
    void destroy(U *p)
    {
        p->~U();
    }

    template<class U, class ... Args>
    void construct(U* p, Args&&... args)
    {
        ::new ((void *) p) U(std::forward<Args>(args)...);
    }

    // std::allocator traits
    typedef T value_type;
    typedef T* pointer;
    typedef T const* const_pointer;
    typedef void* void_pointer;
    typedef void const* const_void_pointer;

private:
    MemoryPool *pool_; // shared
};


template<typename T>
size_t PoolAllocator<T>::size() const
{
    size_t rv = 0;
    for (size_t i = 0; i < pool_->state.size(); ++i)
    {
        if (pool_->state[i] == MemoryPool::MEM_USED)
            ++rv;
    }

    return rv;
}

template<typename T>
T *PoolAllocator<T>::allocate(size_t desired_sz)
{
    if (sizeof(T) != desired_sz)
    {
        throw PoolAllocationDesiredSize();
    }

    for (size_t i = 0; i < pool_->state.size(); ++i)
    {
        if (pool_->state[i] == MemoryPool::MEM_AVAILABLE)
        {
            pool_->state[i] = MemoryPool::MEM_USED;
            return reinterpret_cast<T*>(&pool_->data[0] + i * sizeof(T));
        }
    }

    throw PoolAllocationOutOfMemory();
}

template<typename T>
void PoolAllocator<T>::deallocate(T* ptr, size_t desired_sz)
{
    if (sizeof(T) != desired_sz)
    {
        throw PoolAllocationDesiredSize();
    }

    size_t offset = ptr - reinterpret_cast<const T*>(&pool_->data[0]);
    if (offset < 0 || offset > pool_->state.size())
    {
        throw PoolAllocationSegmentationError();
    }

    if (pool_->state[offset] == MemoryPool::MEM_AVAILABLE)
    {
        throw PoolAllocationDoubleFree();
    }

    pool_->state[offset] = MemoryPool::MEM_AVAILABLE;
}

template<typename T>
PoolAllocator<T>::PoolAllocator(MemoryPool *pool) :
    pool_(pool)
{

}

template<typename T>
PoolAllocator<T>::~PoolAllocator()
{

}

} // practice

namespace std {

template<typename T>
struct allocator_traits <practice::PoolAllocator<T> >
{
    typedef typename practice::PoolAllocator<T> allocator_type;

    // All other values will just use std::allocator
    template<typename U>
    using rebind_alloc = std::allocator<U>;
};

} // std

#endif // _PRACTICE_POOL_ALLOCATOR_
