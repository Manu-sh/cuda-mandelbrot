#pragma once
#include <cstdint>
#include <string_view>

namespace pnm {

    enum: uint8_t {
        BIT_8  = 8,
        BIT_16 = 16,
    };

    enum class Format: uint8_t {
        PPM3, PPM6,
        PGM2, PGM5
    };


    constexpr static inline auto RGB_ALIGNMENT = 32;

    struct alignas(RGB_ALIGNMENT) rgb_t { // 8 bit depth
        uint8_t r;
        uint8_t b;
        uint8_t g;

        uint8_t padding[RGB_ALIGNMENT - 3];
    };

    template <const uint8_t BIT_DEPTH> struct rgb;

    template <>
    struct alignas(RGB_ALIGNMENT) rgb<BIT_8> {
        uint8_t r;
        uint8_t b;
        uint8_t g;

        uint8_t padding[RGB_ALIGNMENT - 3];
    };


    template <>
    struct alignas(RGB_ALIGNMENT) rgb<BIT_16> {
        uint16_t r;
        uint16_t b;
        uint16_t g;

        uint8_t padding[RGB_ALIGNMENT - 3 * 2];
    };

    template <const uint8_t BIT_DEPTH> struct grayscale;

    template <>
    struct __attribute__((__packed__)) grayscale<BIT_8> {
        grayscale() = default;
        inline constexpr grayscale(uint8_t r, uint8_t g, uint8_t b): data(.3 * r + .59 * g + .11 * b) {} // convert from rgb to grayscale using the luminosity method
        inline constexpr grayscale(const rgb<BIT_8> &px): grayscale{px.r, px.b, px.g} {}
        uint8_t data;
    };


    template <>
    struct alignas(RGB_ALIGNMENT) grayscale<BIT_16> {
        grayscale() = default;
        inline constexpr grayscale(uint16_t r, uint16_t g, uint16_t b): data(.3 * r + .59 * g + .11 * b) {} // convert from rgb to grayscale using the luminosity method
        inline constexpr grayscale(const rgb<BIT_16> &px): grayscale{px.r, px.b, px.g} {}
        uint16_t data;
    };


    struct grayscale16_t { uint16_t data; };

    static_assert(
            sizeof(rgb_t) == alignof(rgb_t) &&
            sizeof(rgb_t) == RGB_ALIGNMENT,
            "unexpected size or alignment for rgb_t structure"
    );
}