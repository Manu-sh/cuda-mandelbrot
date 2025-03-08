#pragma once
#include <pnm/pixels/depth.hpp>
#include <cstdint>

namespace pnm {

    template <const uint8_t BIT_DEPTH> struct rgb;

    template <>
    struct __attribute__((__packed__)) rgb<BIT_8> {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    static_assert(sizeof(rgb<BIT_8>) == 3, "rgb<BIT_8> must be exactly 3 byte");
    static_assert(sizeof(rgb<BIT_8>{}.r) + sizeof(rgb<BIT_8>{}.g) + sizeof(rgb<BIT_8>{}.b) == 3, "the size of each rgb<BIT_8> channel MUST be exactly 1 byte");

}