#include <pnm/Header.hpp>
#include <pnm/pnm.hpp>

#include <pnm/ppm/PPM.hpp>
#include <pnm/pnm_common.hpp>

#include <cstddef>
#include <cassert>
#include <cstring>
#include <memory>
#include <new>

const PNM<pnm::rgb<pnm::BIT_8>> & PNM<pnm::rgb<pnm::BIT_8>>::write_ascii(const char *const file_name) const {

    if (!m_length) return *this;

    // +1 for null terminator we actually dont use
    const auto bsize = 12 * m_length + 1; // 12 -> strlen("255 255 255 ")

    std::unique_ptr<uint8_t[]> mem{new uint8_t[bsize]};
    uint8_t *p = mem.get();

    for (uint32_t i = 0; i < m_length; ++i) {
        const pnm::rgb<pnm::BIT_8> px = this->m_vct[i];
        memcpy(p, map_ascii[px.r], map_length[px.r]), p += map_length[px.r];
        memcpy(p, map_ascii[px.g], map_length[px.g]), p += map_length[px.g];
        memcpy(p, map_ascii[px.b], map_length[px.b]), p += map_length[px.b];
    }

    const uint8_t *const beg = mem.get();
    const uint8_t *const end = p;
    assert(end <= &beg[bsize-1]);
    assert((size_t)std::distance(beg, end) <= bsize-1);
    *p = '\0'; // p is writable here

    auto header = pnm::Header<pnm::Format::PPM3, pnm::BIT_8>{m_width, m_height};
    return ::write_file_content(file_name, header, beg, p), *this;
}

const PNM<pnm::rgb<pnm::BIT_8>> & PNM<pnm::rgb<pnm::BIT_8>>::write_binary(const char *const file_name) const {

    if (!m_length) return *this;

    const auto bsize = sizeof(*m_vct) * m_length;
    const uint8_t *const beg = ((uint8_t*)(void *)this->m_vct);
    const uint8_t *const end = ((uint8_t*)(void *)(this->m_vct + m_length));

    assert(end <= (beg + bsize));
    assert((size_t)std::distance(beg, end) <= bsize);
    //printf("%p -> len in bytes: %zu, bytes: %zu\n", beg, bsize, std::distance(beg, end));

    auto header = pnm::Header<pnm::Format::PPM6, pnm::BIT_8>{m_width, m_height};
    return ::write_file_content(file_name, header, beg, end), *this;
}