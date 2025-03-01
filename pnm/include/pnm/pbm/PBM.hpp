#pragma once
#include <type_traits>

#include <pnm/pnm.hpp>
#include <pnm/common.hpp>
#include <pnm/matrix/BitMatrix1D.hpp>

struct PBM: BitMatrix1D<> {

    //static_assert(sizeof(GrayScale{}.data) == 1, "only 8 bit depth is supported, the size of each channel MUST be exactly 1 byte");

    using BitMatrix1D<>::BitMatrix1D;
    //using pixel_type = GrayScale;

};