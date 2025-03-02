#pragma once
#include <type_traits>

#include <pnm/pnm.hpp>
#include <pnm/common.hpp>
#include <pnm/matrix/StandardMatrix1D.hpp>

template <typename GrayScale, typename Enable = std::enable_if_t<std::is_same_v<GrayScale, pnm::grayscale<pnm::BIT_8>> ||std::is_same_v<GrayScale, pnm::grayscale<pnm::BIT_16>>>>
struct PGM: StandardMatrix1D<GrayScale> {

    static_assert(sizeof(GrayScale{}.data) == 1, "only 8 bit depth is supported, the size of each channel MUST be exactly 1 byte");

    using StandardMatrix1D<GrayScale>::StandardMatrix1D;
    using pixel_type = GrayScale;

    template <pnm::Format format>
    constexpr FORCED(inline) const PGM & write_file_content(const char *const file_name) const {
        if constexpr(format == pnm::Format::PGM2)
            return write_file_content_pnm2(file_name), *this;

        return write_file_content_pnm5(file_name), *this;
    }

    private:
        const PGM & write_file_content_pnm2(const char *const file_name) const;
        const PGM & write_file_content_pnm5(const char *const file_name) const;
};