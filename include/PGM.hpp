#pragma once
#include <type_traits>

#include <pixels.hpp>
#include <AlignedMatrix1D.hpp>

template <typename GrayScale, typename Enable = std::enable_if_t<std::is_same_v<GrayScale, pixels::grayscale8_t> || std::is_same_v<GrayScale, pixels::grayscale16_t>>>
class PGM: AlignedMatrix1D<GrayScale> {

    using AlignedMatrix1D<GrayScale>::AlignedMatrix1D;

};