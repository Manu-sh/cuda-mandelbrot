#pragma once
#include <cstddef>
#include <cstdint>
#include <cassert>

// #include <type_traits>
// template <typename std::enable_if<std::is_unsigned_v<T>, T>::type>

constexpr bool is_power_of_2(size_t n) {
    return n > 0 && !(n & (n - 1));
}

// calc the closest number to desired_bsize aligned to alignment,
// es. f(1, 32) -> 32, f(33, 32) -> 64
template <const size_t ALIGNMENT>
constexpr auto aligned_bsize_calc(size_t desired_bsize) {
    static_assert(is_power_of_2(ALIGNMENT), "invalid alignment value for ALIGNMENT (should be a power of 2)");
    return ((desired_bsize + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT;
}
