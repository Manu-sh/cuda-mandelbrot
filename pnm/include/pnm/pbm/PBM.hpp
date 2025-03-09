#pragma once
#include <cstdint>
#include <type_traits>

#include <pnm/pnm.hpp>
#include <pnm/pixels/monochrome.hpp>

#include <pnm/common.hpp>
#include <pnm/matrix/BitMatrix1D.hpp>

template <>
struct PNM<pnm::monochrome_t>: private BitMatrix1D {

    using BitMatrix1D::BitMatrix1D;
    using BitMatrix1D::operator();

    uint16_t height() const noexcept { return BitMatrix1D::height(); }
    uint16_t  width() const noexcept { return BitMatrix1D::width();  }
    uint32_t length() const noexcept { return BitMatrix1D::length(); }
    uint64_t  bsize() const noexcept { return BitMatrix1D::bsize();  }

    const BitArray8 * unwrap() const noexcept { return BitMatrix1D::unwrap(); }
          BitArray8 * unwrap()       noexcept { return BitMatrix1D::unwrap(); }

    bool operator()(uint16_t r, uint16_t c) const {
        return BitMatrix1D::operator()(r, c);
    }

    void operator()(uint16_t r, uint16_t c, pnm::monochrome_t px) {
        BitMatrix1D::operator()(r, c, px.data);
    }

    const PNM & write_file_content(const char *const file_name, bool use_ascii_fmt = 0) const {
        return use_ascii_fmt ? write_ascii(file_name) : write_binary(file_name);
    }

    protected:
        const PNM & write_ascii(const char *const file_name) const;
        const PNM & write_binary(const char *const file_name) const;
};