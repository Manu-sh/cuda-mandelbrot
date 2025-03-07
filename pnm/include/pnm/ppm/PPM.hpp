#pragma once
#include <type_traits>

#include <pnm/pnm.hpp>
#include <pnm/pnm/IPNM.hpp>

#include <pnm/common.hpp>
#include <pnm/matrix/StandardMatrix1D.hpp>

// https://en.wikipedia.org/wiki/Netpbm

template <>
struct PNM<pnm::rgb<pnm::BIT_8>>:
        IPNM<pnm::rgb<pnm::BIT_8>>,
        StandardMatrix1D<pnm::rgb<pnm::BIT_8>> {

    using RGB = pnm::rgb<pnm::BIT_8>;
    using StandardMatrix1D<RGB>::StandardMatrix1D;

    static_assert(sizeof(RGB{}.r) + sizeof(RGB{}.g) + sizeof(RGB{}.b) == 3, "only 8 bit depth is supported, the size of each channel MUST be exactly 1 byte");

    const PNM & write_ascii(const char *const file_name) const;
    const PNM & write_binary(const char *const file_name) const;

    uint16_t height() const noexcept override { return StandardMatrix1D::height(); }
    uint16_t width()  const noexcept override { return StandardMatrix1D::width(); }
    uint32_t length() const noexcept override { return StandardMatrix1D::length(); }
    uint64_t bsize()  const noexcept override { return StandardMatrix1D::bsize(); }

    void operator()(uint16_t r, uint16_t c, RGB px) override { StandardMatrix1D::operator()(r, c) = px; }
    RGB operator()(uint16_t r, uint16_t c) const override { return StandardMatrix1D::operator()(r, c); }
};