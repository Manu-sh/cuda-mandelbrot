#pragma once

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <memory>
#include <new>
#include <iostream>

using std::cout, std::endl;

#include <pnm/matrix/AbstractMatrix1D.hpp>
#include <pnm/matrix/BitArray8.hpp>

template <typename Allocator = std::allocator<BitArray8>>
class BitMatrix1D: public AbstractMatrix1D<BitArray8> {

    // same of (int)ceil(x/8.)
    static constexpr FORCED(inline) uint16_t ceil_div(uint16_t num, uint8_t div) {
        return (num-1) / div + 1;
    }

    public:
        using allocator_type  = Allocator;
        using allocator_trait = std::allocator_traits<allocator_type>;

    public:
        BitMatrix1D(uint16_t width, uint16_t height): AbstractMatrix1D<BitArray8>{width, height} {

            //this->m_length = width * height;
            this->m_length = 0; // TODO:
            this->m_byte_height = height;
            this->m_byte_width  = ceil_div(width, 8);
            this->m_byte_length = m_byte_width * height; // padded_rows * height

            this->m_vct = (BitArray8 *)allocator_trait::allocate(m_allocator, this->m_byte_length);

            /*
            if constexpr(std::is_constructible<BitArray8>::value) {
                for (uint_fast32_t i = 0; i < this->m_byte_length; ++i)
                    allocator_trait::construct(m_allocator, this->m_vct + i);
            }*/

        }

    ~BitMatrix1D() {

        /*
        if constexpr(std::is_destructible<BitArray8>::value) {
            for (uint_fast32_t i = 0; i < this->m_byte_length; ++i)
                allocator_trait::destroy(m_allocator, this->m_vct + i);
        }*/

        allocator_trait::deallocate(m_allocator, this->m_vct, this->m_length);
        this->m_vct = (BitArray8 *)(long)(this->m_length = this->m_width = this->m_height = 0);
        this->m_byte_length = this->m_byte_width = this->m_byte_height = 0;
    }


        uint8_t operator()(uint16_t r, uint16_t c) const {

            const uint32_t byte_idx = r * m_byte_width + (c >> 3); //  r * m_byte_width + (c/8)
            if (byte_idx >= m_byte_length || r >= m_height || c >= m_width)
                throw std::runtime_error("index out of bound");

            const BitArray8 &b = m_vct[byte_idx];
            return b[c&7]; // b[c%8];
        }

        uint8_t operator()(uint16_t r, uint16_t c, uint8_t value) {
#if 0
            cout << "\nm_byte_length: " << m_byte_length << endl;
            cout << "m_byte_height: " << m_byte_height << endl;
            cout << "m_byte_width:  " << m_byte_width << endl;
            cout << "r: " << r << endl;
            cout << "c: " << c << endl;
#endif

            const uint32_t byte_idx = r * m_byte_width + (c >> 3); //  r * m_byte_width + (c/8)
            // cout << "byte_idx: " << byte_idx << endl;

            if (byte_idx >= m_byte_length || r >= m_height || c >= m_width)
                throw std::runtime_error("index out of bound");

            BitArray8 &b = m_vct[byte_idx]; //cout <<"m_vct["<<byte_idx<<"] -> b[" << c%8 << "] = " << (int)value << endl;
            return b(c&7, value); // b(c%8, value) -> b(c - ((c >> 3) << 3), value) -> b(c & 7, value)
        }

    protected:
        allocator_type m_allocator;

        uint32_t m_byte_height;
        uint32_t m_byte_width;
        uint32_t m_byte_length;
};
