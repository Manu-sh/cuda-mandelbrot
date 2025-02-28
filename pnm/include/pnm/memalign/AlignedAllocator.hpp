#pragma once
#include <cstddef>
#include <new>

#include <pnm/memalign/utils.hpp>

template <typename T, const size_t ALIGNMENT>
struct AlignedAllocator {

    // passing a size in bytes which is not a multiple of ALIGNMENT result in UB
    // for example aligned_alloc(32, 65) and aligned_alloc(32, 63) are UB
    // while aligned_alloc(32, 64) is correct
    static_assert(is_power_of_2(ALIGNMENT), "invalid alignment value for ALIGNMENT (should be a power of 2)"); // required by all decent implementations
    static_assert(is_multiple_of_word(ALIGNMENT), "invalid alignment value for ALIGNMENT (should be a multiple of sizeof(void*))"); // posix_memalign require this, many new() operator implementations use posix_memalign under the hood so it's required
    static_assert(ALIGNMENT >= alignof(std::max_align_t), "some implementation require this to be >= alignof(std::max_align_t) however by the standard there are is no minimal"); // suggested for portability

    constexpr static auto alignment = ALIGNMENT;
    using value_type = T;

    inline T * allocate(size_t length) { // allocate but not construct the element
        const auto aligned_bsize = aligned_bsize_calc<ALIGNMENT>(length * sizeof(T));
        T *p = (T *)::operator new[](aligned_bsize, std::align_val_t(ALIGNMENT));
        if (!p || ((long)p) % ALIGNMENT != 0) throw std::bad_alloc{}; // "incorrectly aligned memory"
        return p;
    }

    inline void deallocate(T *p, [[maybe_unused]] size_t n = 0) noexcept { // deallocate but do not destruct the element (void *) prevent destruct to be called for T
        //if (((long)p) % ALIGNMENT != 0) throw std::bad_alloc{}; // "incorrectly aligned memory"
        ::operator delete[]((void *)p, std::align_val_t(ALIGNMENT));
    }

    template <typename U>
    struct rebind {
        using other = AlignedAllocator<U, ALIGNMENT>;
    };

};