#pragma once
#include <cstddef>
#include <pnm/common.hpp>

constexpr FORCED(inline) bool is_power_of_2(size_t n) {
    return n > 0 && !(n & (n - 1));
}

constexpr FORCED(inline) bool is_multiple_of_word(size_t n) {
    return !(n % sizeof(void *));
}

// calc the closest number to desired_bsize aligned to alignment,
// es. f(1, 32) -> 32, f(33, 32) -> 64
// https://en.cppreference.com/w/c/memory/aligned_alloc#Notes
template <const size_t ALIGNMENT>
constexpr FORCED(inline) auto aligned_bsize_calc(size_t desired_bsize) {
    static_assert(is_power_of_2(ALIGNMENT), "invalid alignment value for ALIGNMENT (should be a power of 2)"); // required by all decent implementations
    static_assert(is_multiple_of_word(ALIGNMENT), "invalid alignment value for ALIGNMENT (should be a multiple of sizeof(void*))"); // posix_memalign require this, many new() operator implementations use posix_memalign under the hood so it's required
    static_assert(ALIGNMENT >= alignof(std::max_align_t), "some implementation require this to be >= alignof(std::max_align_t) however by the standard there are is no minimal"); // suggested for portability
    return ((desired_bsize + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT;
}


/*

// ceil $num up to the next multiple value of $to -> ceil_up(11, 10) -> 20
static constexpr FORCED(inline) uint16_t ceil_up(uint16_t num, uint8_t to) {
    return num == 0 ? to : ((num + to - 1) / to) * to;
}

*/