#pragma once
#include <cstdint>

struct __attribute__((__packed__)) BitArray8 {

    private:
        inline BitArray8 & set_bit(uint8_t i) {
            data |= (1 << (7-i));
            return *this;
        }

        inline BitArray8 & clear_bit(uint8_t i) {
            data &= ~(1 << (7-i));
            return *this;
        }

    public:
        inline uint8_t operator[](uint8_t i) const {
            assert(i < 8);
            return (data >> (7 - i)) & 1;
        }

        inline uint8_t operator()(uint8_t i, uint8_t value) {
            return (value ? this->set_bit(i) : this->clear_bit(i))[i];
        }

    private:
        uint8_t data;
};

static_assert(sizeof(BitArray8) == 1, "BitArray8 must be exactly 1 byte");