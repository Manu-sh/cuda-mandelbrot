#pragma once
#include <cstdint>
#include <stdexcept>
#include <new>

#include <memalign/utils.hpp>
#include <common.hpp>

// memory aligned to 256 bit (32 byte) to use avx2
template <typename T, const size_t ALIGNMENT = sizeof(T)>
class Matrix1D {

    static_assert(is_power_of_2(ALIGNMENT), "invalid alignment value for ALIGNMENT (should be a power of 2)");
    static_assert(is_multiple_of_word(ALIGNMENT), "invalid alignment value for ALIGNMENT (should be a multiple of sizeof(void*))");

    public:
        static constexpr size_t matrix_cell_alignment = ALIGNMENT; // A static data member declared constexpr on its first declaration is implicitly an inline variable.
        using matrix_cell_type = T;

    private:
        Matrix1D(const Matrix1D &o) = delete;
        Matrix1D & operator=(const Matrix1D &o) = delete;

    public:

        explicit Matrix1D(uint16_t height, uint16_t width);
        ~Matrix1D() {
            //delete[] m_vct;
            operator delete[](m_vct, std::align_val_t(ALIGNMENT));
        }

        T & at(uint16_t r, uint16_t c);
        FORCED(inline) T * unwrap()  noexcept { return m_vct; }
        FORCED(inline) uint16_t height() const noexcept { return m_height; }
        FORCED(inline) uint16_t  width() const noexcept { return m_width;  }
        FORCED(inline) uint32_t length() const noexcept { return m_length; }
        FORCED(inline) uint64_t  bsize() const noexcept { return sizeof(T) * m_length; }

        FORCED(inline) T & operator()(uint16_t r, uint16_t c) noexcept { return m_vct[r * m_width + c]; }
        FORCED(inline) const T & operator()(uint16_t r, uint16_t c) const noexcept { return m_vct[r * m_width + c]; }

        FORCED(inline) decltype(auto) at(uint16_t r, uint16_t c) const {
            constexpr Matrix1D<T> *const self = (Matrix1D<T>*)this; // avoid infinite recursion
            if constexpr (sizeof(T) <= sizeof(long))
                return (T)self->at(r, c);
            else
                return (const T &)self->at(r, c);
        }

        FORCED(inline) const T * unwrap() const {
            constexpr Matrix1D<T> *const self = (Matrix1D<T>*)this; // avoid infinite recursion
            return (const T *)self->unwrap();
        }

    protected:
        T *m_vct = nullptr;
        uint32_t m_length;
        uint16_t m_width;
        uint16_t m_height;
};

template <typename T, const size_t ALIGNMENT>
Matrix1D<T, ALIGNMENT>::Matrix1D(uint16_t height, uint16_t width)
        : m_width{width}, m_height{height}, m_length{(uint32_t)width*height} {

/*
    m_vct = (T *)__builtin_assume_aligned(
            new (std::align_val_t(ALIGNMENT)) T[aligned_bsize_calc<ALIGNMENT>(
                    aligned_bsize_calc<ALIGNMENT>(width) * height // align every row
            )],
            ALIGNMENT
    );
*/
    m_vct = new (std::align_val_t(ALIGNMENT)) T[m_length];

    // passing a size in bytes which is not a multiple of ALIGNMENT result in UB
    // for example aligned_alloc(32, 65) and aligned_alloc(32, 63) are UB
    // while aligned_alloc(32, 64) is correct
    if (((long)m_vct) % ALIGNMENT != 0)
        throw std::runtime_error("incorrectly aligned memory");
}

template <typename T, const size_t ALIGNMENT>
T & Matrix1D<T, ALIGNMENT>::at(uint16_t r, uint16_t c) {

    const uint32_t i = r * m_width + c; // row major order
    if (i >= m_length)
        throw std::runtime_error("index out of bound");

    return m_vct[i];
}