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

    const auto len = m_width * m_height;
    const auto bsize = 2 * len + 1; // 4 -> strlen("0 ") + 1 for the null terminator we actually dont use

    std::unique_ptr<uint8_t[]> mem{new uint8_t[bsize]};
    uint8_t *p = mem.get();

    const uint16_t chunked_width = m_width / 8; // contiguos bytes
    const uint16_t remaind_width = m_width % 8; // number of bits remaining to read before encountering the padding

    for (uint32_t i = 0, c = 0; i < m_byte_length; i += m_byte_width, c = 0) {

        // copy the line fetching by byte until there is no padding
        for (; c < chunked_width; ++c) {

            //cout << "byte[" << i << "]" << " of p[" << bsize << "]" << endl;
            const BitArray8 bit = this->m_vct[i]; // this skip many checks
            #pragma GCC unroll 8
            for (uint8_t i = 0; i < 8; ++i) {
                *p++ = (bit[i] + '0'); // convert o ascii
                *p++ = ' ';
            }
        }

        if (!remaind_width) continue;

        //cout << "byte[" << i << "]" << "-> (" << remaind_width << ") bits" << endl;

        // read remaining bits and stop before padding
        const BitArray8 bit = this->m_vct[i];
        for (; c < remaind_width; ++c) {
            *p++ = (bit[c] + '0');
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
