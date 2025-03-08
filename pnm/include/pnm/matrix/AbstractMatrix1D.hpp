#pragma once
#include <cstdint>
#include <stdexcept>
#include <pnm/common.hpp>

template <typename T>
struct AbstractMatrix1D {

    public:
        using value_type      = T;
        using difference_type = int64_t;
        using pointer         = T*;
        using reference       = T&;

    private:
        AbstractMatrix1D(const AbstractMatrix1D &o)             = delete;
        AbstractMatrix1D & operator=(const AbstractMatrix1D &o) = delete;
        AbstractMatrix1D(AbstractMatrix1D &&o)                  = delete;
        AbstractMatrix1D & operator=(AbstractMatrix1D &&o)      = delete;

    protected:
        AbstractMatrix1D() = default;
        explicit AbstractMatrix1D(uint16_t width, uint16_t height)
            : m_length{(uint32_t)width*height}, m_width{width}, m_height{height} { }

    public:
        FORCED(inline) uint16_t height() const noexcept { return m_height; }
        FORCED(inline) uint16_t  width() const noexcept { return m_width;  }

        FORCED(inline) uint32_t length() const noexcept { return m_length; }
        FORCED(inline) uint64_t  bsize() const noexcept { return sizeof(T) * m_length; } // internal buffer size in bytes

        FORCED(inline) T & operator()(uint16_t r, uint16_t c) noexcept { return m_vct[r * m_width + c]; }
        FORCED(inline) const T & operator()(uint16_t r, uint16_t c) const noexcept { return m_vct[r * m_width + c]; }

        FORCED(inline) decltype(auto) at(uint16_t r, uint16_t c) const {
            AbstractMatrix1D<T> *const self = (AbstractMatrix1D<T>*)this; // avoid infinite recursion
            if constexpr (sizeof(T) <= sizeof(long))
                return (T)self->at(r, c);
            else
                return (const T &)self->at(r, c);
        }

        FORCED(inline) T * unwrap() noexcept { return m_vct; }

        FORCED(inline) const T * unwrap() const noexcept {
            AbstractMatrix1D<T> *const self = (AbstractMatrix1D<T>*)this; // avoid infinite recursion
            return (const T *)self->unwrap();
        }

        T & at(uint16_t r, uint16_t c) {

            const uint32_t i = r * m_width + c; // row major order
            if (i >= m_length)
                throw std::runtime_error("index out of bound");

            return m_vct[i];
        }

    protected:
        T *m_vct = nullptr;
        uint32_t m_length;
        uint16_t m_width;
        uint16_t m_height;
};

