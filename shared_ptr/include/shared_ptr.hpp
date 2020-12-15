#ifndef _SHARED_PTR_PRACTICE_
#define _SHARED_PTR_PRACTICE_

#include <memory>  // std::allocator
#include <cassert>

#if __GNUC__ < 4
#error "GCC version is not at least version 4!"
#endif

#if __GNUC__ == 4 && __GNUC_MINOR__ < 8
#error "GCC version must be >=4.8! "
#endif

namespace practice
{

template<class T>
struct RemoveReference
{
    typedef T Type;
};

template<class T>
struct RemoveReference<T&>
{
    typedef T Type;
};

template<class T>
struct RemoveReference<T&&>
{
    typedef T Type;
};


template< class T >
constexpr typename RemoveReference<T>::Type&& move( T&& t )
{
    return static_cast<typename RemoveReference<T>::Type&&>(t);
}

template<typename T, typename Alloc = std::allocator<T> >
class SharedPtr : private Alloc
{
public:
    SharedPtr();

    explicit SharedPtr(T* ptr);

    SharedPtr(T* ptr, Alloc const &alloc);

    ~SharedPtr(); // non-virtual

    // copy/assign
    SharedPtr &operator=(SharedPtr const &other);
    SharedPtr(SharedPtr const &other);

    // move
    SharedPtr &operator=(SharedPtr &&other);
    SharedPtr(SharedPtr &&other);

    void reset();

    void swap(SharedPtr &other);

    T& operator*()              { assert(ptr_); return *ptr_; }
    T const & operator*() const { assert(ptr_); return *ptr_; }

    T* operator->()              { assert(ptr_); return ptr_; }
    T const* operator->() const  { assert(ptr_); return ptr_; }

    int count() const    { return (count_) ? *count_ : 0; }
    T const *get() const { return ptr_; }

    explicit operator bool() const { return ptr_ != 0; }

private:
    // We need to allocate the count pointer with our Alloc
    // Specialized allocators should use allocator_traits to define this type
    typedef typename std::allocator_traits<Alloc>::template rebind_alloc<int> AllocInt;

    int* count_;
    T* ptr_;
};

template<typename T, typename Alloc>
SharedPtr<T, Alloc>::SharedPtr() :
    SharedPtr<T, Alloc>(nullptr, Alloc())
{

}

template<typename T, typename Alloc>
SharedPtr<T, Alloc>::SharedPtr(T* ptr) :
    SharedPtr<T, Alloc>(ptr, Alloc()) // delegating constructor
{

}

template<typename T, typename Alloc>
SharedPtr<T, Alloc>::SharedPtr(T* ptr, Alloc const &alloc) :
    Alloc(alloc),
    count_(nullptr),
    ptr_(nullptr)
{
    if (nullptr != ptr)
    {
        AllocInt a2;
        count_ = a2.allocate(sizeof(int));
        Alloc::construct(count_);
        *count_ = 1;
        ptr_ = ptr;
    }
}

template<typename T, typename Alloc>
SharedPtr<T, Alloc> &SharedPtr<T, Alloc>::operator=(SharedPtr<T, Alloc> &&other)
{
    // Alloc has no state, cannot be assigned
    if (other.ptr_ == ptr_)
    {
        return *this;
    }

    reset();
    swap(other);

    return *this;
}

template<typename T, typename Alloc>
SharedPtr<T, Alloc>::SharedPtr(SharedPtr<T, Alloc> &&other) :
    Alloc(practice::move(other)),
    count_(practice::move(other.count_)),
    ptr_(practice::move(other.ptr_))
{
    other.count_ = nullptr;
    other.ptr_   = nullptr;
}

template<typename T, typename Alloc>
SharedPtr<T, Alloc>::SharedPtr(SharedPtr<T, Alloc> const &other) :
    Alloc(other)
{
    count_ = other.count_;
    ptr_   = other.ptr_;

    ++(*count_);
}

template<typename T, typename Alloc>
SharedPtr<T, Alloc>::~SharedPtr()
{
    reset();
}

template<typename T, typename Alloc>
SharedPtr<T, Alloc> &SharedPtr<T, Alloc>::operator=(SharedPtr<T, Alloc> const &other)
{
    // Self assignment
    if (other.ptr_ == ptr_)
    {
        ptr_   = other.ptr_;
        count_ = other.count_;
        return *this;
    }

    reset();

    SharedPtr<T, Alloc> new_p(other);
    swap(new_p);

    return *this;
}

template<typename T, typename Alloc>
void SharedPtr<T, Alloc>::reset()
{
    if (count_)
    {
        --(*count_);

        if (0 == *count_)
        {
            AllocInt a2;

            a2.destroy(count_);
            a2.deallocate(count_, sizeof(int));

            Alloc::destroy(ptr_);
            Alloc::deallocate(ptr_, sizeof(T));
        }
    }

    count_ = nullptr;
    ptr_   = nullptr;
}

template<typename T, typename Alloc>
void SharedPtr<T, Alloc>::swap(SharedPtr<T, Alloc> &other)
{
    T *tmp = ptr_;
    ptr_ = other.ptr_;
    other.ptr_ = tmp;

    int *tmp_c = count_;
    count_ = other.count_;
    other.count_ = tmp_c;
}

} // practice

#endif
