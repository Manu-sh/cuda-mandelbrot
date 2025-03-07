#pragma once
#include <type_traits>

#include <pnm/pnm.hpp>
#include <pnm/common.hpp>
#include <pnm/matrix/StandardMatrix1D.hpp>

// https://en.wikipedia.org/wiki/Netpbm

template <>
struct PNM<pnm::rgb<pnm::BIT_8>>: public StandardMatrix1D<pnm::rgb<pnm::BIT_8>> {

    using RGB = pnm::rgb<pnm::BIT_8>;
    using StandardMatrix1D<RGB>::StandardMatrix1D;

    static_assert(sizeof(RGB{}.r) + sizeof(RGB{}.g) + sizeof(RGB{}.b) == 3, "only 8 bit depth is supported, the size of each channel MUST be exactly 1 byte");

    const PNM & write_ascii(const char *const file_name) const;
    const PNM & write_binary(const char *const file_name) const;
};