#pragma once
#include <cstddef>
#include <new>
#include <cstdlib>
#include <memory>
#include <memalign/utils.hpp>

#include <iostream>

using std::cout, std::endl;

template <typename T, const size_t ALIGNMENT>

struct al_allocator: std::allocator<T> {

    static_assert(is_power_of_2(ALIGNMENT), "invalid alignment value for ALIGNMENT (should be a power of 2)");

    // ((desired_bsize + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT
   // constexpr static size_t alignment = ALIGNMENT;
    using aligned_type = T;

#if 0
    inline constexpr T * allocate(size_t n) {

        return std::allocator<T>::allocate(n);
        const auto aligned_bsize = (((n * sizeof(T)) + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT;

#if 0
        cout << "alignment " << ALIGNMENT << " size: " << n << " aligned_bsize: " << aligned_bsize << endl;

        if (aligned_bsize % ALIGNMENT != 0)
            throw std::runtime_error("internal error: incorrectly aligned memory"); // this is a fatal internal library error so result in std::terminate() due to noexcept clause is fine
#endif

        T *p = (T*)std::aligned_alloc(ALIGNMENT, aligned_bsize);
        if (!p) throw std::bad_alloc{};
        return p;
    }

    inline void deallocate(T *p, [[maybe_unused]] size_t n) {
        free(p);
    }
#endif


    template <typename U>
    struct rebind {
        using other = al_allocator<U, ALIGNMENT>;
    };

};
