#pragma once
#include <cstdint>

template <typename T>
struct IMatrix {
    virtual IMatrix() = default;
    virtual uint16_t height()  const noexcept = 0;
    virtual uint16_t  width()  const noexcept = 0;
    virtual uint32_t length()  const noexcept = 0;

    virtual const T * unwrap() const noexcept = 0;
    virtual uint64_t  bsize()  const noexcept = 0; // internal buffer size

    virtual T & operator()(uint16_t r, uint16_t c) noexcept = 0;
    virtual const T & operator()(uint16_t r, uint16_t c) const noexcept = 0;
    //virtual T at(uint16_t r, uint16_t c) const = 0;
};