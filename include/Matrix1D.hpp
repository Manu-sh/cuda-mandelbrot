#pragma once
#include <cstdint>
#include <cstdlib>
#include <stdexcept>
#include <type_traits>
#include <new>

#if 0
    // test to run with -O0
    #include <iostream>
    using std::cout, std::endl;
    int main() {

        struct __attribute__((__packed__)) big_t {
            big_t() {
                p = malloc(113);
                cout << "constructor called, pointer " << p << '\n';
            }
            ~big_t() {
                free(p);
                cout << "called for pointer " << p << '\n';
            }
            void *p = nullptr;
            char buf[11];
        };

        cout << sizeof(big_t) << endl;
        Matrix1D<big_t, 4> mtx{3, 2};
        //Matrix1D<char, 3> mtx2{3, 3}; // should raise an error at compilation time
        Matrix1D<char, 1> mtx2{3, 3};
        return 0;
    }
#endif

// memory aligned to 256 bit (32 byte) to use avx2
template <typename T, const size_t ALIGNMENT = sizeof(T)>
class Matrix1D {

    public:
        constexpr static size_t matrix_type_alignment = ALIGNMENT;
        using matrix_type = T;

    private:
        Matrix1D(const Matrix1D &o) = delete;
        Matrix1D & operator=(const Matrix1D &o) = delete;

    public:

        explicit Matrix1D(uint16_t height, uint16_t width);
        ~Matrix1D() {
            if (!m_vct) return;

            // ::operator delete[] (nullptr, m_vct);
            // -> according to stroustrup (Programming: Principles and Practice Using C++)
            // this operator definition to nothing (unless overloads) except inform the garbage collector,
            // so just call manually every destructor

            if constexpr(std::is_destructible<T>::value) {
                for (uint_fast32_t i = 0; i < m_length; ++i)
                    m_vct[i].~T();
            }

            free(m_vct);
        }

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

template <typename T, const size_t ALIGNMENT>
Matrix1D<T, ALIGNMENT>::Matrix1D(uint16_t height, uint16_t width)
        : m_width{width}, m_height{height}, m_length{(uint32_t)width*height} {

    constexpr auto is_power_of_2 = [] (decltype(ALIGNMENT) a) -> bool {
        return a > 0 && !(a & (a - 1));
    };

    constexpr auto aligned_bsize_calc = [] (size_t bsize) -> size_t {
        return ((bsize + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT;
    };

    static_assert(is_power_of_2(ALIGNMENT), "invalid alignment value for ALIGNMENT (should be a power of 2)");

    const auto aligned_bsize = aligned_bsize_calc(sizeof(T) * m_length);
    //printf("allocating block of %zu aligned to %zu\n", aligned_bsize, ALIGNMENT);

    m_vct = (decltype(m_vct)) aligned_alloc(ALIGNMENT, aligned_bsize);
    new (m_vct) T[m_length]; // initialize the memory block calling in-place new[]

    // passing a size in bytes which is not a multiple of ALIGNMENT result in UB
    // for example aligned_alloc(32, 65) and aligned_alloc(32, 63) are UB
    // while aligned_alloc(32, 64) is correct
    if (((long)m_vct) % ALIGNMENT != 0 || aligned_bsize % ALIGNMENT != 0)
        throw std::runtime_error("incorrectly aligned memory");
}

template <typename T, const size_t ALIGNMENT>
T & Matrix1D<T, ALIGNMENT>::at(uint16_t r, uint16_t c) {

    const uint32_t i = r * m_width + c; // row major order
    if (i >= m_length)
        throw std::runtime_error("index out of bound");

    return m_vct[i];
}