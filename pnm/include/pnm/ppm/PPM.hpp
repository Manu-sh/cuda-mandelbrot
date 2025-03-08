#pragma once
#include <cstdint>
#include <type_traits>

#include <pnm/pnm.hpp>
#include <pnm/pixels/rgb.hpp>

#include <pnm/common.hpp>
#include <pnm/matrix/StandardMatrix1D.hpp>

// https://en.wikipedia.org/wiki/Netpbm

template <>
struct PNM<pnm::rgb<pnm::BIT_8>>: private StandardMatrix1D<pnm::rgb<pnm::BIT_8>> {

    using RGB = pnm::rgb<pnm::BIT_8>;
    using StandardMatrix1D<RGB>::StandardMatrix1D;

    static_assert(sizeof(RGB{}.r) + sizeof(RGB{}.g) + sizeof(RGB{}.b) == 3, "only 8 bit depth is supported, the size of each channel MUST be exactly 1 byte");

    uint16_t height() const noexcept { return StandardMatrix1D::height(); }
    uint16_t  width() const noexcept { return StandardMatrix1D::width();  }
    uint32_t length() const noexcept { return StandardMatrix1D::length(); }
    uint64_t  bsize() const noexcept { return StandardMatrix1D::bsize();  }

    const RGB * unwrap() const noexcept { return StandardMatrix1D::unwrap(); }
          RGB * unwrap()       noexcept { return StandardMatrix1D::unwrap(); }

    const RGB & operator()(uint16_t r, uint16_t c) const noexcept { return StandardMatrix1D::operator()(r, c); }
          RGB & operator()(uint16_t r, uint16_t c)       noexcept { return StandardMatrix1D::operator()(r, c); }

    const PNM & write_file_content(const char *const file_name, bool use_ascii_fmt = 0) const {
        return use_ascii_fmt ? write_ascii(file_name) : write_binary(file_name);
    }

    protected:
        const PNM & write_ascii(const char *const file_name) const;
        const PNM & write_binary(const char *const file_name) const;
};