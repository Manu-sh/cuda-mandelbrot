#pragma once
#include <cstdint>
#include <type_traits>

#include <pnm/pnm.hpp>
#include <pnm/pixels/grayscale.hpp>

#include <pnm/common.hpp>
#include <pnm/matrix/StandardMatrix1D.hpp>

template <>
struct PNM<pnm::grayscale<pnm::BIT_8>>: private StandardMatrix1D<pnm::grayscale<pnm::BIT_8>> {

    using GrayScale = pnm::grayscale<pnm::BIT_8>;
    using StandardMatrix1D<GrayScale>::StandardMatrix1D;

    static_assert(sizeof(GrayScale{}.data) == 1, "only 8 bit depth is supported, the size of each channel MUST be exactly 1 byte");

    uint16_t height() const noexcept { return StandardMatrix1D::height(); }
    uint16_t  width() const noexcept { return StandardMatrix1D::width();  }
    uint32_t length() const noexcept { return StandardMatrix1D::length(); }
    uint64_t  bsize() const noexcept { return StandardMatrix1D::bsize();  }

    const GrayScale * unwrap() const noexcept { return StandardMatrix1D::unwrap(); }
          GrayScale * unwrap()       noexcept { return StandardMatrix1D::unwrap(); }

    const GrayScale & operator()(uint16_t r, uint16_t c) const noexcept { return StandardMatrix1D::operator()(r, c); }
          GrayScale & operator()(uint16_t r, uint16_t c)       noexcept { return StandardMatrix1D::operator()(r, c); }

    const PNM & write_file_content(const char *const file_name, bool use_ascii_fmt = 0) const {
        return use_ascii_fmt ? write_ascii(file_name) : write_binary(file_name);
    }

    protected:
        const PNM & write_ascii(const char *const file_name) const;
        const PNM & write_binary(const char *const file_name) const;
};