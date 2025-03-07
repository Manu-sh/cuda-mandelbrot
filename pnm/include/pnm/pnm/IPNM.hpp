#pragma once
#include <cstdint>

template <typename Pixel>
struct IPNM {

    virtual ~IPNM() = default;

    virtual uint16_t height() const noexcept = 0;
    virtual uint16_t  width() const noexcept = 0;

    virtual void  operator()(uint16_t r, uint16_t c, Pixel value) = 0;
    virtual Pixel operator()(uint16_t r, uint16_t c) = 0;

    virtual const IPNM &  write_ascii(const char *const file_name) const = 0;
    virtual const IPNM & write_binary(const char *const file_name) const = 0;
};