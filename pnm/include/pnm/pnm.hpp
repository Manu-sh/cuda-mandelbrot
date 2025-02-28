#pragma once
#include <cstdint>
#include <string_view>
#include <cassert>

#include <pnm/common.hpp>

namespace pnm {

    // return the number of bit a row should have to be aligned at 8 bit (1 byte)
    static constexpr FORCED(inline) uint16_t row_bit_length_calc(uint16_t row_bit_len) {
        constexpr uint8_t ALIGNMENT = 8; // 8 bit
        return ((row_bit_len + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT;
    }

    static constexpr FORCED(inline) uint8_t row_byte_padding(uint16_t row_bit_length) {
        int should_be_bits = row_bit_length_calc(row_bit_length);
        return (should_be_bits - row_bit_length) / 8; // 8/ -> the size returned is how much BYTE padding we need on this row
    }


    enum: uint8_t {
        BIT_8  = 8,
        BIT_16 = 16,
    };

    enum class Format: uint8_t {
        PPM3, PPM6,
        PGM2, PGM5
    };

    // 3x2 -> vct[3 * (3*2) + 2]
    struct __attribute__((__packed__)) BN {

        // ((0b0101_0101 >> i)&1).to_s(2)
        inline uint8_t operator[](uint8_t i) const {
            assert(i < 8);
            return (data >> (7 - i)) & 1;
        }

        // 2x4 -> height x width
        inline uint8_t operator()(uint8_t r, uint8_t c) const {
            static constexpr auto cols = 2;
            return this[0][r * cols + c];
        }

        uint8_t data;
    };

    template <const uint8_t BIT_DEPTH> struct rgb;

    template <>
    struct __attribute__((__packed__)) rgb<BIT_8> {
        uint8_t r;
        uint8_t b;
        uint8_t g;
    };

    template <>
    struct __attribute__((__packed__)) rgb<BIT_16> {
        uint16_t r;
        uint16_t b;
        uint16_t g;
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
    struct __attribute__((__packed__)) grayscale<BIT_16> {
        grayscale() = default;
        inline constexpr grayscale(uint16_t r, uint16_t g, uint16_t b): data(.3 * r + .59 * g + .11 * b) {} // convert from rgb to grayscale using the luminosity method
        inline constexpr grayscale(const rgb<BIT_16> &px): grayscale{px.r, px.b, px.g} {}
        uint16_t data;
    };

}