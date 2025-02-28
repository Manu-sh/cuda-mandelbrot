#include <pnm/pnm_common.hpp>
#include <pnm/pgm/PGM.hpp>
#include <pnm/Header.hpp>
#include <pnm/pnm.hpp>

#include <cstddef>
#include <cassert>
#include <cstring>
#include <memory>
#include <new>

template <>
const PGM<pnm::grayscale<pnm::BIT_8>> & PGM<pnm::grayscale<pnm::BIT_8>>::write_file_content_ppm2(const char *const file_name) const {

    const auto bsize = 4 * m_length + 1; // 12 -> strlen("255 ") + 1 for the null terminator we actually dont use
    std::unique_ptr<char[]> ret{new char[bsize]};
    uint8_t *const beg = (uint8_t *)(void *)ret.get();
    char *p = (char *)(void *)beg;

    for (uint32_t i = 0; i < m_length; ++i) {
        const pnm::grayscale<pnm::BIT_8> px = this->m_vct[i];
        memcpy(p, map_ascii[px.data], map_length[px.data]), p += map_length[px.data];
    }

    uint8_t *const end = ((uint8_t*)(void *)p);
    assert(end <= &beg[bsize-1]);
    assert((size_t)std::distance(beg, end) <= bsize-1);
    *p = '\0'; // p is writable here

    auto header = pnm::Header<pnm::Format::PGM2, pnm::BIT_8>{m_width, m_height};
    return ::write_file_content(file_name, header, beg, end), *this;
}

template <>
const PGM<pnm::grayscale<pnm::BIT_8>> & PGM<pnm::grayscale<pnm::BIT_8>>::write_file_content_ppm5(const char *const file_name) const {

    // last iteration has move the pointer by sizeof(rgb3_t) -> 3 bytes, which is okay because both are big at least a multiple of 3 bytes
    const uint8_t *const beg = ((uint8_t*)(void *)this->m_vct);
    const uint8_t *const end = ((uint8_t*)(void *)(this->m_vct + this->m_length));
    assert((size_t)std::distance(beg, end) <= this->m_length);
    //printf("%p -> len in bytes: %zu, bytes: %zu\n", beg, this->m_length, std::distance(beg, end));

    auto header = pnm::Header<pnm::Format::PGM5, pnm::BIT_8>{m_width, m_height};
    return ::write_file_content(file_name, header, beg, end), *this;
}
