#pragma once
#include <type_traits>

#include <pnm/pnm.hpp>
#include <pnm/common.hpp>
#include <pnm/matrix/StandardMatrix1D.hpp>

template <>
struct PNM<pnm::grayscale<pnm::BIT_8>>: StandardMatrix1D<pnm::grayscale<pnm::BIT_8>> {

    using GrayScale = pnm::grayscale<pnm::BIT_8>;
    using StandardMatrix1D<GrayScale>::StandardMatrix1D;
    static_assert(sizeof(GrayScale{}.data) == 1, "only 8 bit depth is supported, the size of each channel MUST be exactly 1 byte");

    const PNM & write_ascii(const char *const file_name) const;
    const PNM & write_binary(const char *const file_name) const;
};