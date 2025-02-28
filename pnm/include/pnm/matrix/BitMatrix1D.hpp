#pragma once

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <memory>
#include <new>

#include <pnm/matrix/AbstractMatrix1D.hpp>

struct BitArray8;

// (r=1) * (cols=(1920/8)) + (c=(1919/8))

template <typename Allocator = std::allocator<BitArray8>>
class BitMatrix1D: public AbstractMatrix1D<BitArray8> {

    public:

        struct __attribute__((__packed__)) BitArray8 {

            // ((0b0101_0101 >> i)&1).to_s(2)
            inline uint8_t operator[](uint8_t i) const {
                assert(i < 8);
                return (data >> (7 - i)) & 1;
            }

            // 2x4 -> height x width
            inline uint8_t operator()(uint8_t r, uint8_t c) const {
                static constexpr auto cols = 2;
                return this[0][r * cols + c];
            }

            uint8_t data;
        };

        using allocator_type  = Allocator;
        using allocator_trait = std::allocator_traits<allocator_type>;

    public:
        BitMatrix1D(uint16_t width, uint16_t height): AbstractMatrix1D<BitArray8>{width, height} {

            //this->m_length = ..;

            this->m_vct = (T *)allocator_trait::allocate(m_allocator, this->m_length);
            if constexpr(std::is_constructible<T>::value) {
                for (uint_fast32_t i = 0; i < this->m_length; ++i)
                    allocator_trait::construct(m_allocator, this->m_vct + i);
            }

        }

    protected:
        allocator_type m_allocator;
};

