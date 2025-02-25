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
