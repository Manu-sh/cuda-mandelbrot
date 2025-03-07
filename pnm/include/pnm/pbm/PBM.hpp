#pragma once
#include <cstdint>
#include <type_traits>

#include <pnm/pnm.hpp>
#include <pnm/pnm/IPNM.hpp>
#include <pnm/common.hpp>
#include <pnm/matrix/BitMatrix1D.hpp>

//  IPNM<pnm::monochrome_t>

template <>
struct PNM<pnm::monochrome_t>: BitMatrix1D<> {

    using BitMatrix1D<>::BitMatrix1D;
    using BitMatrix1D<>::operator();

    void operator()(uint16_t r, uint16_t c, const pnm::monochrome_t px) {
        BitMatrix1D::operator()(r, c, px.data);
    }

    const PNM & write_ascii(const char *const file_name) const;
    const PNM & write_binary(const char *const file_name) const;
};