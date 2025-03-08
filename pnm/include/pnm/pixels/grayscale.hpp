#pragma once
#include <cstdint>
#include <pnm/pnm.hpp>
#include <pnm/pixels/depth.hpp>
#include <pnm/pixels/rgb.hpp>

namespace pnm {

    template <const uint8_t BIT_DEPTH> struct grayscale;

    template <>
    struct __attribute__((__packed__)) grayscale<BIT_8> {
        grayscale() = default;
        inline constexpr grayscale(uint8_t r, uint8_t g, uint8_t b): data(.3 * r + .59 * g + .11 * b) {} // convert from rgb to grayscale using the luminosity method
        inline constexpr grayscale(const rgb<BIT_8> &px): grayscale{px.r, px.g, px.b} {}
        uint8_t data;
    };

    static_assert(sizeof(grayscale<BIT_8>) == 1, "grayscale<BIT_8> must be exactly 1 byte");
    static_assert(sizeof(grayscale<BIT_8>{}.data), "the size of the grayscale<BIT_8> channel MUST be exactly 1 byte");
}