#pragma once
#include <cstdint>
#include <string_view>
#include <cassert>

#include <pnm/common.hpp>
#include <pnm/pixels/rgb.hpp>
#include <pnm/pixels/grayscale.hpp>

template <typename Pixel> struct PNM;

namespace pnm {
    using rgb_t = rgb<BIT_8>;
    using grayscale_t = grayscale<BIT_8>;
}