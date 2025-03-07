#pragma once
#include <cstdint>
#include <string_view>
#include <cassert>

#include <pnm/common.hpp>

template <typename Pixel> struct PNM;

namespace pnm {


    enum: uint8_t {
        BIT_2  = 2,
        BIT_8  = 8,
        //BIT_16 = 16,
    };

    enum class Format: uint8_t {
        PPM3, PPM6,
        PGM2, PGM5,
        PBM4, PBM1
    };

    template <const uint8_t BIT_DEPTH> struct rgb;

    template <>
    struct __attribute__((__packed__)) rgb<BIT_8> {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    static_assert(sizeof(rgb<BIT_8>) == 3, "rgb<BIT_8> must be exactly 3 byte");
    static_assert(sizeof(rgb<BIT_8>{}.r) + sizeof(rgb<BIT_8>{}.g) + sizeof(rgb<BIT_8>{}.b) == 3, "the size of each rgb<BIT_8> channel MUST be exactly 1 byte");

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

    struct __attribute__((__packed__)) monochrome_t: grayscale<BIT_8> {

        enum: uint8_t { WHITE, BLACK }; // PBM use 0 for white and 1 for black

        monochrome_t() = default;
        inline constexpr monochrome_t(uint8_t r, uint8_t g, uint8_t b): grayscale<BIT_8>{r, g, b}  {
            this->data = data <= 128 ? BLACK : WHITE; // convert from grayscale to monochromatic
        }

        inline constexpr monochrome_t(const rgb<BIT_8> &px): monochrome_t{px.r, px.g, px.b} {}
    };

    static_assert(sizeof(monochrome_t) == 1, "monochrome_t must be exactly 1 byte");
    static_assert(sizeof(monochrome_t{}.data), "the size of the monochrome_t channel MUST be exactly 1 byte");

    #if 0
        template <>
        struct __attribute__((__packed__)) rgb<BIT_16> {
            uint16_t r;
            uint16_t g;
            uint16_t b;
        };

        template <>
        struct __attribute__((__packed__)) grayscale<BIT_16> {
            grayscale() = default;
            inline constexpr grayscale(uint16_t r, uint16_t g, uint16_t b): data(.3 * r + .59 * g + .11 * b) {} // convert from rgb to grayscale using the luminosity method
            inline constexpr grayscale(const rgb<BIT_16> &px): grayscale{px.r, px.g, px.b} {}
            uint16_t data;
        };
    #endif


}