#include <pnm/pnm_common.hpp>
#include <pnm/pbm/PBM.hpp>
#include <pnm/Header.hpp>
#include <pnm/pnm.hpp>

#include <cstddef>
#include <cassert>
#include <cstring>
#include <memory>
#include <new>

#include <iostream>

using namespace std;

template <>
const PBM<pnm::monochrome_t> & PBM<pnm::monochrome_t>::write_file_content_pnm1(const char *const file_name) const {

    // last iteration has move the pointer by sizeof(rgb3_t) -> 3 bytes, which is okay because both are big at least a multiple of 3 bytes
    const uint8_t *const beg = ((uint8_t*)(void *)this->m_vct);
    const uint8_t *const end = ((uint8_t*)(void *)(this->m_vct + this->m_byte_length));
    assert((size_t)std::distance(beg, end) <= this->m_byte_length);
    //printf("%p -> len in bytes: %zu, bytes: %zu\n", beg, this->m_length, std::distance(beg, end));

    auto header = pnm::Header<pnm::Format::PBM4, pnm::BIT_2>{m_width, m_height};
    return ::write_file_content(file_name, header, beg, end), *this;
}

template <>
const PBM<pnm::monochrome_t> & PBM<pnm::monochrome_t>::write_file_content_pnm4(const char *const file_name) const {

    // 2 -> strlen("0 ") + 1 for the null terminator we actually dont use
    const auto bsize = 2 * (m_width * m_height) + 1;

    std::unique_ptr<uint8_t[]> mem{new uint8_t[bsize]};
    uint8_t *p = mem.get();

    const uint16_t chunked_width = m_width / 8; // contiguous bytes
    const uint16_t remaind_width = m_width % 8; // number of bits remaining to read before encountering the padding

    for (uint32_t i = 0, c = 0; i < m_byte_length; i += m_byte_width, c = 0) {

        // for each element of the current row

        // copy the line fetching by byte until there is no padding
        for (; c < chunked_width; ++c) {
            const BitArray8 bit = this->m_vct[i+c]; // this skip many checks
            assert(i+c < m_byte_length);

            *p++ = (bit[0] + '0'), *p++ = ' '; // convert o ascii every bit
            *p++ = (bit[1] + '0'), *p++ = ' ';
            *p++ = (bit[2] + '0'), *p++ = ' ';
            *p++ = (bit[3] + '0'), *p++ = ' ';
            *p++ = (bit[4] + '0'), *p++ = ' ';
            *p++ = (bit[5] + '0'), *p++ = ' ';
            *p++ = (bit[6] + '0'), *p++ = ' ';
            *p++ = (bit[7] + '0'), *p++ = ' ';
        }

        if (!remaind_width) continue;

        assert(i+c < m_byte_length);

        // read remaining bits and stop before padding
        const BitArray8 bit = this->m_vct[i+c];
        for (uint8_t b = 0; b < remaind_width; ++b) {
            *p++ = (bit[b] + '0');
            *p++ = ' ';
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
