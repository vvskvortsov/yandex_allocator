#ifndef POOLALLOCATORCPP_H
#define POOLALLOCATORCPP_H


#include  <cstddef>
#include <exception>
#include <iostream>
#include "pool_allocator.h"


/*!
 * Just example of wrapper for using allocator in cpp. Allocator requirements are met.
 *
 * This wrapper can be used with standart cpp containers (vector for example, see main).
 *
 * You can disable exceptions by defining PA_CONFIG_NO_EXCEPTIONS.
 */


template<typename T>
class PoolAllocatorCPP {
public:
    PoolAllocatorCPP() = default;

    typedef T value_type;


    inline bool operator==(const PoolAllocatorCPP& )
    {
        return true;
    }
    inline bool operator!=(const PoolAllocatorCPP& rhs)
    {
        return !(this == rhs);
    }


    template <typename U>
    PoolAllocatorCPP(const PoolAllocatorCPP<U>&)
    {
    }

    template <typename U>
    struct rebind {
        typedef PoolAllocatorCPP<U> other;
    };


    [[nodiscard]] T* allocate(std::size_t n)
    {
        if (n * sizeof(T) > PA_CONFIG_BLOCK_SIZE) {

#ifndef PA_CONFIG_NO_EXCEPTIONS
            throw std::bad_alloc();
#endif
            return nullptr;
        }

        if (auto p = static_cast<T*>(pa_allocate_block())) {
            return p;
        }

#ifndef PA_CONFIG_NO_EXCEPTIONS
        throw std::bad_alloc();
#endif
        return nullptr;
    }

    void deallocate(T* p, std::size_t) noexcept
    {
        pa_deallocate(static_cast<void*>(p));
    }

};

#endif // POOLALLOCATORCPP_H
