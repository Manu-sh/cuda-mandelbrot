#pragma once
#include <cstdint>
#include <string>
#include <string_view>
#include <fstream>

#include <pnm/pnm.hpp>
#include <pnm/matrix/AlignedMatrix1D.hpp>
#include <pnm/memalign/utils.hpp>

// https://en.wikipedia.org/wiki/Netpbm
struct PPM: public AlignedMatrix1D<pnm::rgb_t> {

    using rgb_t = pnm::rgb_t;

    static_assert(sizeof(rgb_t{}.r) + sizeof(rgb_t{}.b) + sizeof(rgb_t{}.g) == 3, "only 8 bit depth is supported, the size of each channel MUST be exactly 1 byte");

    using Matrix     = AlignedMatrix1D<rgb_t>;
    using pixel_type = typename Matrix::value_type;
    using Matrix::Matrix;

    static constexpr auto pixel_type_alignment = Matrix::cell_alignment; // sizeof(rgb_t) == alignof(rgb_t)

    template <pnm::Format format>
    constexpr FORCED(inline) const PPM & write_file_content(const char *const file_name) const {
        if constexpr(format == pnm::Format::PPM3)
            return write_file_content_ppm3(file_name);

        return write_file_content_ppm6(file_name);
    }

    private:
        const PPM & write_file_content_ppm3(const char *const file_name) const;
        const PPM & write_file_content_ppm6(const char *const file_name) const;
};

