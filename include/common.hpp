#pragma once
#include <cstdint>
#include <cuda_runtime.h>

// https://stackoverflow.com/questions/2151084/map-a-2d-array-onto-a-1d-array
constexpr uint32_t AT(uint16_t cols, uint16_t r, uint16_t c) { // cols = width = x_size
    return r * cols + c;
}

__device__ uint32_t ATfast(uint16_t cols, uint16_t r, uint16_t c) { // cols = width = x_size
    return __umul24(r, cols) + c; //return r * cols + c;
}

struct alignas(uint32_t) rgb_t {
    uint8_t r;
    uint8_t b;
    uint8_t g;
};

static_assert(sizeof(rgb_t) == 4, "ooops");