#pragma once
#include <cstdint>

// https://stackoverflow.com/questions/2151084/map-a-2d-array-onto-a-1d-array
constexpr uint32_t AT(uint16_t cols, uint16_t r, uint16_t c) { // cols = width = x_size
    return r * cols + c;
}

struct alignas(32) rgb_t {
    uint8_t r;
    uint8_t b;
    uint8_t g;
};

//static_assert(sizeof(rgb_t) == 4, "ooops");