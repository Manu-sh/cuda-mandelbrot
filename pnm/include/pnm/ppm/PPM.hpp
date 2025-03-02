#pragma once
#include <cstdint>
#include <string>

#include <pnm/pnm.hpp>
#include <pnm/matrix/StandardMatrix1D.hpp>
#include <pnm/common.hpp>

// https://en.wikipedia.org/wiki/Netpbm

template <typename RGB, typename Enable = std::enable_if_t<std::is_same_v<RGB, pnm::rgb<pnm::BIT_8>> ||std::is_same_v<RGB, pnm::rgb<pnm::BIT_16>>>>
struct PPM: public StandardMatrix1D<RGB> {

    static_assert(sizeof(RGB{}.r) + sizeof(RGB{}.g) + sizeof(RGB{}.b) == 3, "only 8 bit depth is supported, the size of each channel MUST be exactly 1 byte");

    using Matrix     = StandardMatrix1D<RGB>;
    using pixel_type = typename Matrix::value_type;
    using Matrix::Matrix;

    template <pnm::Format format>
    constexpr FORCED(inline) const PPM & write_file_content(const char *const file_name) const {
        if constexpr(format == pnm::Format::PPM3)
            return write_file_content_pnm3(file_name);

        return write_file_content_pnm6(file_name);
    }

    private:
        const PPM & write_file_content_pnm3(const char *const file_name) const;
        const PPM & write_file_content_pnm6(const char *const file_name) const;
};

