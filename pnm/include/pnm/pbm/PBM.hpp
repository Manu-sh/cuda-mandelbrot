#pragma once
#include <cstdint>
#include <type_traits>

#include <pnm/pnm.hpp>
#include <pnm/pnm/IPNM.hpp>

#include <pnm/common.hpp>
#include <pnm/matrix/BitMatrix1D.hpp>

//  IPNM<pnm::monochrome_t>

template <>
struct PNM<pnm::monochrome_t>: public IPNM<pnm::monochrome_t>, private BitMatrix1D {

    using BitMatrix1D::BitMatrix1D;
    using BitMatrix1D::operator();

    void operator()(uint16_t r, uint16_t c, pnm::monochrome_t px) override {
        BitMatrix1D::operator()(r, c, px.data);
    }

    pnm::monochrome_t operator()(uint16_t r, uint16_t c) const override {
        return pnm::monochrome_t{ BitMatrix1D::operator()(r, c) };
    }


    uint16_t height() const noexcept override { return BitMatrix1D::height(); }
    uint16_t width()  const noexcept override { return BitMatrix1D::width(); }
    uint32_t length() const noexcept override { return BitMatrix1D::length(); }
    uint64_t bsize()  const noexcept override { return BitMatrix1D::bsize(); }

    BitArray8 * unwrap() noexcept { return BitMatrix1D::unwrap(); }
    const BitArray8 * unwrap() const noexcept { return BitMatrix1D::unwrap(); }

    const PNM & write_ascii(const char *const file_name) const;
    const PNM & write_binary(const char *const file_name) const;
};