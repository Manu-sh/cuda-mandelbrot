#pragma once
#include <cstdint>
#include <string>
#include <string_view>
#include <fstream>

#include <pnm/pnm.hpp>
#include <pnm/matrix/AlignedMatrix1D.hpp>
#include <pnm/memalign/utils.hpp>

// https://en.wikipedia.org/wiki/Netpbm

template <typename RGB, typename Enable = std::enable_if_t<std::is_same_v<RGB, pnm::rgb<pnm::BIT_8>> ||std::is_same_v<RGB, pnm::rgb<pnm::BIT_16>>>>
struct PPM: public AlignedMatrix1D<RGB> {

    static_assert(sizeof(RGB{}.r) + sizeof(RGB{}.b) + sizeof(RGB{}.g) == 3, "only 8 bit depth is supported, the size of each channel MUST be exactly 1 byte");

    using Matrix     = AlignedMatrix1D<RGB>;
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

