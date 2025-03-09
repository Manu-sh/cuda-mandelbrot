#pragma once
#include <cstdint>
#include <pnm/pnm.hpp>
#include <pnm/pixels/depth.hpp>
#include <pnm/pixels/grayscale.hpp>

namespace pnm {

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
}