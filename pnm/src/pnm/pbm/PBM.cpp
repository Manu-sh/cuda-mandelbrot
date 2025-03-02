#include <pnm/pnm_common.hpp>
#include <pnm/pbm/PBM.hpp>
#include <pnm/Header.hpp>
#include <pnm/pnm.hpp>

#include <cstddef>
#include <cassert>
#include <cstring>
#include <memory>
#include <new>

template <>
const PBM<pnm::monochrome_t> & PBM<pnm::monochrome_t>::write_file_content_pnm4(const char *const file_name) const {

    static_assert(sizeof(BitArray8) == 1, "ooops");

    if (!(m_width && m_height))
        return *this;

    // 2 -> strlen("0 ") + 1 for the null terminator we actually dont use
    const auto bsize = 2 * (m_width * m_height) + 1;

    std::unique_ptr<uint8_t[]> mem{new uint8_t[bsize]};
    uint8_t *p = mem.get();

    const uint16_t chunked_width = m_width / 8; // contiguous bytes containing just pixels with no pad garbage
    const uint16_t remaind_width = m_width % 8; // number of bits remaining to read before encountering the padding bits

    if (!remaind_width)
        goto fast_cpy;

    for (uint32_t i = 0, w_byte = 0; i < m_byte_length; i += m_byte_width) {

        const BitArray8 *pr = this->m_vct + i;

        // for each element of the current row
        // copy the line fetching by byte until there is padding
        for (w_byte = 0; w_byte < chunked_width; ++w_byte, ++pr) {
            const BitArray8 bit = *pr; // this skip many checks
            *p++ = (bit[0] + '0'), *p++ = ' '; // convert o ascii every bit
            *p++ = (bit[1] + '0'), *p++ = ' ';
            *p++ = (bit[2] + '0'), *p++ = ' ';
            *p++ = (bit[3] + '0'), *p++ = ' ';
            *p++ = (bit[4] + '0'), *p++ = ' ';
            *p++ = (bit[5] + '0'), *p++ = ' ';
            *p++ = (bit[6] + '0'), *p++ = ' ';
            *p++ = (bit[7] + '0'), *p++ = ' ';
        }

        // it's 8 multiple there is no padding
        assert(remaind_width);
        assert(pr < m_vct+m_byte_length);

        // read remaining bits and stop before padding
        const BitArray8 bit = *pr;
        for (uint8_t b = 0; b < remaind_width; ++b) {
            *p++ = (bit[b] + '0');
            *p++ = ' ';
        }

    }

    if (false) {
        fast_cpy:
        const BitArray8 *bit = this->m_vct;
        for (uint32_t i = 0; i < m_byte_length; ++i) {
            *p++ = (bit[0][0] + '0'), *p++ = ' ';
            *p++ = (bit[0][1] + '0'), *p++ = ' ';
            *p++ = (bit[0][2] + '0'), *p++ = ' ';
            *p++ = (bit[0][3] + '0'), *p++ = ' ';
            *p++ = (bit[0][4] + '0'), *p++ = ' ';
            *p++ = (bit[0][5] + '0'), *p++ = ' ';
            *p++ = (bit[0][6] + '0'), *p++ = ' ';
            *p++ = (bit[0][7] + '0'), *p++ = ' ';
        }
    }

    const uint8_t *const beg = mem.get();
    const uint8_t *const end = p;
    assert(end <= &beg[bsize-1]);
    assert((size_t)std::distance(beg, end) <= bsize-1);
    *p = '\0'; // p is writable here

    auto header = pnm::Header<pnm::Format::PBM1, pnm::BIT_2>{m_width, m_height};
    return ::write_file_content(file_name, header, beg, end), *this;
}

template <>
const PBM<pnm::monochrome_t> & PBM<pnm::monochrome_t>::write_file_content_pnm1(const char *const file_name) const {

    if (!(m_width && m_height))
        return *this;

    // last iteration has move the pointer by sizeof(rgb3_t) -> 3 bytes, which is okay because both are big at least a multiple of 3 bytes
    const uint8_t *const beg = ((uint8_t*)(void *)this->m_vct);
    const uint8_t *const end = ((uint8_t*)(void *)(this->m_vct + this->m_byte_length));
    assert((size_t)std::distance(beg, end) <= this->m_byte_length);
    //printf("%p -> len in bytes: %zu, bytes: %zu\n", beg, this->m_length, std::distance(beg, end));

    auto header = pnm::Header<pnm::Format::PBM4, pnm::BIT_2>{m_width, m_height};
    return ::write_file_content(file_name, header, beg, end), *this;
}
