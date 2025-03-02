#pragma once
#include <type_traits>

#include <pnm/pnm.hpp>
#include <pnm/common.hpp>
#include <pnm/matrix/BitMatrix1D.hpp>

template <typename MonoChrome, typename Enable = std::enable_if_t<std::is_same_v<MonoChrome, pnm::monochrome_t>>>
struct PBM: BitMatrix1D<> {

    using BitMatrix1D<>::BitMatrix1D;
    using BitMatrix1D<>::operator();

    void operator()(uint16_t r, uint16_t c, const MonoChrome px) {
        BitMatrix1D::operator()(r, c, px.data);
    }

    template <pnm::Format format>
    constexpr FORCED(inline) const PBM & write_file_content(const char *const file_name) const {
        if constexpr(format == pnm::Format::PBM4)
            return write_file_content_pnm4(file_name), *this;

        return write_file_content_pnm1(file_name), *this;
    }

    private:
        const PBM & write_file_content_pnm1(const char *const file_name) const;
        const PBM & write_file_content_pnm4(const char *const file_name) const;

};