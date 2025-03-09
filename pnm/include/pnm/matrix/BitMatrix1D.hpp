#pragma once

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <memory>
#include <new>

#include <pnm/matrix/AbstractMatrix1D.hpp>
#include <pnm/matrix/BitArray8.hpp>

class BitMatrix1D: public AbstractMatrix1D<BitArray8> {

    // same of (int)ceil(x/8.)
    static constexpr FORCED(inline) uint16_t ceil_div(uint16_t num, uint8_t div) {
        return (num-1) / div + 1;
    }

    public:
        using allocator_type  = std::allocator<BitArray8>;
        using allocator_trait = std::allocator_traits<allocator_type>;

    public:
        BitMatrix1D(uint16_t width, uint16_t height): AbstractMatrix1D<BitArray8>{width, height} {

            // "Each row is Width bits, packed 8 to a byte": https://netpbm.sourceforge.net/doc/pbm.html
            // that means a matrix w=1 h=1080 require 1080 bytes whereas one with w=1920 h=1 just 240 bytes
            this->m_byte_height = height;
            this->m_byte_width  = ceil_div(width, 8);
            this->m_length = m_byte_width * m_byte_height; // padded_rows * height
            this->m_vct = (BitArray8 *)allocator_trait::allocate(m_allocator, this->m_length);
        }

        ~BitMatrix1D() {
            allocator_trait::deallocate(m_allocator, this->m_vct, this->m_length);
            this->m_vct = (BitArray8 *)(long)(this->m_length = this->m_width = this->m_height = 0);
            this->m_byte_width = this->m_byte_height = 0;
        }

        bool operator()(uint16_t r, uint16_t c) const {

            const uint32_t byte_idx = r * m_byte_width + (c >> 3); //  r * m_byte_width + (c/8)
            if (byte_idx >= m_length || r >= m_height || c >= m_width)
                throw std::runtime_error("index out of bound");

            const BitArray8 &bit_a = m_vct[byte_idx];
            return bit_a[c&7]; // b[c%8];
        }

        void operator()(uint16_t r, uint16_t c, bool value) {

            const uint32_t byte_idx = r * m_byte_width + (c >> 3);
            if (byte_idx >= m_length || r >= m_height || c >= m_width)
                throw std::runtime_error("index out of bound");

            BitArray8 &bit_a = m_vct[byte_idx];
            return (void)bit_a(c&7, value);     // bit(c%8, value) -> bit(c - ((c >> 3) << 3), value) -> bit(c & 7, value)
        }

    protected:
        allocator_type m_allocator;
        uint16_t m_byte_height;
        uint16_t m_byte_width;
};
