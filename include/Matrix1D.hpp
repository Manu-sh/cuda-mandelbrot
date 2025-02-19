#pragma once
#include <cstdint>
#include <stdexcept>

template <typename T>
class Matrix1D {

    public:
        using matrix_type = T;

    private:
        Matrix1D(const Matrix1D &o) = delete;
        Matrix1D & operator=(const Matrix1D &o) = delete;

    public:
        explicit Matrix1D(uint16_t height, uint16_t width);
        ~Matrix1D() { delete[] m_vct; }

        T & at(uint16_t r, uint16_t c);
        inline T * unwrap() { return m_vct; }
        inline uint16_t height() const noexcept { return m_height; }
        inline uint16_t  width() const noexcept { return m_width;  }
        inline uint32_t length() const noexcept { return m_length; }
        inline uint64_t  bsize() const noexcept { return sizeof(T) * m_length; }

        inline T & operator()(uint16_t r, uint16_t c) noexcept { return m_vct[r * m_width + c]; }
        inline const T & operator()(uint16_t r, uint16_t c) const noexcept { return m_vct[r * m_width + c]; }

        decltype(auto) at(uint16_t r, uint16_t c) const {
            Matrix1D<T> *const self = (Matrix1D<T>*)this; // avoid infinite recursion
            if constexpr (sizeof(T) <= sizeof(long))
                return (T)self->at(r, c);
            else
                return (const T &)self->at(r, c);
        }

        const T * unwrap() const {
            Matrix1D<T> *const self = (Matrix1D<T>*)this; // avoid infinite recursion
            return (const T *)self->unwrap();
        }

    protected:
        T *m_vct = nullptr;
        uint32_t m_length;
        uint16_t m_width;
        uint16_t m_height;
};

template <typename T>
Matrix1D<T>::Matrix1D(uint16_t height, uint16_t width)
        : m_width{width}, m_height{height}, m_length{(uint32_t)width*height} {

    m_vct = new T[m_length];
}

template <typename T>
T & Matrix1D<T>::at(uint16_t r, uint16_t c) {

    const uint32_t i = r * m_width + c; // row major order
    if (i >= m_length)
        throw std::runtime_error("index out of bound");

    return m_vct[i];
}