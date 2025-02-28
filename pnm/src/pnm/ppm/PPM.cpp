#include <pnm/Header.hpp>
#include <pnm/pnm.hpp>

#include <pnm/ppm/PPM.hpp>
#include <pnm/pnm_common.hpp>

#include <cstddef>
#include <cassert>
#include <cstring>
#include <memory>
#include <new>


template <>
const PPM<pnm::rgb<pnm::BIT_8>> & PPM<pnm::rgb<pnm::BIT_8>>::write_file_content_ppm3(const char *const file_name) const {

    static const uint8_t pixel_length = strlen("255 255 255 ");

    // +1 for null terminator we actually dont use but we need *p
    // to be writeable to make use of std::distance(mem, last)
    const auto bsize = pixel_length * m_length + 1;
    std::unique_ptr<char[]> ret{new char[bsize]};
    char *const mem = ret.get();
    char *p = mem;

    /*
    for (uint16_t r = 0; r < m_height; ++r) {
        for (uint16_t c = 0; c < m_width; ++c) {
            const pnm::rgb<pnm::BIT_8> px = this->operator()(r, c);
            memcpy(p, map_ascii[px.r], map_length[px.r]), p += map_length[px.r];
            memcpy(p, map_ascii[px.b], map_length[px.b]), p += map_length[px.b];
            memcpy(p, map_ascii[px.g], map_length[px.g]), p += map_length[px.g];
        }
    }
    */


    for (uint32_t i = 0; i < m_length; ++i) {
        const pnm::rgb<pnm::BIT_8> px = this->m_vct[i];
        memcpy(p, map_ascii[px.r], map_length[px.r]), p += map_length[px.r];
        memcpy(p, map_ascii[px.b], map_length[px.b]), p += map_length[px.b];
        memcpy(p, map_ascii[px.g], map_length[px.g]), p += map_length[px.g];
    }


    assert(p <= &mem[bsize-1]);
    assert((size_t)std::distance(mem, p) <= bsize-1);
    *p = '\0'; // p is writable here

    auto header = pnm::Header<pnm::Format::PPM3, pnm::BIT_8>{m_width, m_height};
    return ::write_file_content(file_name, header, (uint8_t *)mem, (uint8_t *)p), *this;
}


template <>
const PPM<pnm::rgb<pnm::BIT_8>> & PPM<pnm::rgb<pnm::BIT_8>>::write_file_content_ppm6(const char *const file_name) const {

    const auto bsize = sizeof(*m_vct) * m_length;
    const uint8_t *const beg = ((uint8_t*)(void *)this->m_vct);
    const uint8_t *const end = ((uint8_t*)(void *)(this->m_vct + m_length));

    assert(end <= (beg + bsize));
    assert((size_t)std::distance(beg, end) <= bsize);
    //printf("%p -> len in bytes: %zu, bytes: %zu\n", beg, bsize, std::distance(beg, end));

    auto header = pnm::Header<pnm::Format::PPM6, pnm::BIT_8>{m_width, m_height};
    return ::write_file_content(file_name, header, beg, end), *this;
}