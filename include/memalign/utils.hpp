#pragma once
#include <cstddef>
#include <common.hpp>

constexpr FORCED(inline) bool is_power_of_2(size_t n) {
    return n > 0 && !(n & (n - 1));
}

constexpr FORCED(inline) bool is_multiple_of_word(size_t n) {
    return !(n % sizeof(void *));
}

// calc the closest number to desired_bsize aligned to alignment,
// es. f(1, 32) -> 32, f(33, 32) -> 64
template <const size_t ALIGNMENT>
constexpr FORCED(inline) auto aligned_bsize_calc(size_t desired_bsize) {
    static_assert(is_power_of_2(ALIGNMENT), "invalid alignment value for ALIGNMENT (should be a power of 2)");
    static_assert(is_multiple_of_word(ALIGNMENT), "invalid alignment value for ALIGNMENT (should be a multiple of sizeof(void*))");
    return ((desired_bsize + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT;
}
