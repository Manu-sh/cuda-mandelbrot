#pragma once
#include <cstdint>

// i could use template specialization and sfinae to force inlining
// for some of these global function, but it's unnecessarily
// complicated and less flexible

#ifdef FORCED
    #error "FORCED() macro already defined"
#endif

#define FORCED(_UNUSED_) __attribute__((always_inline))


// https://stackoverflow.com/questions/2151084/map-a-2d-array-onto-a-1d-array
constexpr FORCED(inline) uint32_t AT(uint16_t cols, uint16_t r, uint16_t c) { // cols = width = x_size
    return r * cols + c;
}

constexpr static inline auto RGB_ALIGNMENT = 32;
struct alignas(RGB_ALIGNMENT) rgb_t {
    uint8_t r;
    uint8_t b;
    uint8_t g;

    uint8_t padding[RGB_ALIGNMENT-3];
};

static_assert(
        sizeof(rgb_t) == alignof(rgb_t) &&
        sizeof(rgb_t) == RGB_ALIGNMENT,
        "unexpected size or alignment for rgb_t structure"
);