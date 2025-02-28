#include <pnm/Header.hpp>
#include <pnm/pnm.hpp>

#include <pnm/ppm/PPM.hpp>
#include <pnm/pnm_common.hpp>

#include <cstddef>
#include <cassert>
#include <cstring>
#include <iterator>
#include <memory>
#include <new>


const PPM & PPM::write_file_content_ppm3(const char *const file_name) const {

    // +1 for null terminator we actually dont use but we need *p
    // to be writeable to make use of std::distance(mem, last)
#if 0
    const auto bsize = aligned_bsize_calc<PPM::pixel_type_alignment>(12 * m_length + 1); // 12 -> strlen("255 255 255 ") + 1 for the null terminator we actually dont use
    alignas(PPM::pixel_type_alignment) char *const mem = new (std::align_val_t(PPM::pixel_type_alignment)) char[bsize];
    static constexpr auto deleter_fix = [](char *const v) { operator delete[](v, std::align_val_t(PPM::pixel_type_alignment)); };
    std::unique_ptr<char[], decltype(deleter_fix)> ret{mem, deleter_fix};
#else
    const auto bsize = 12 * m_length + 1; // 12 -> strlen("255 255 255 ")
    std::unique_ptr<char[]> ret{new char[bsize]};
    char *const mem = ret.get();
#endif

    char *p = mem;


    for (uint16_t r = 0; r < m_height; ++r) {
        for (uint16_t c = 0; c < m_width; ++c) {
            const rgb_t px = this->operator()(r, c);
            memcpy(p, map_ascii[px.r], map_length[px.r]), p += map_length[px.r];
            memcpy(p, map_ascii[px.b], map_length[px.b]), p += map_length[px.b];
            memcpy(p, map_ascii[px.g], map_length[px.g]), p += map_length[px.g];
        }
    }

    assert(p <= &mem[bsize-1]);
    assert((size_t)std::distance(mem, p) <= bsize-1);
    *p = '\0'; // p is writable here

    auto header = pnm::Header<pnm::Format::PPM3, pnm::BIT_8>{m_width, m_height};
    return ::write_file_content(file_name, header, (uint8_t *)mem, (uint8_t *)p), *this;
}


const PPM & PPM::write_file_content_ppm6(const char *const file_name) const {

    // TODO: questo è valido solo per MAXVAL=255 ovvero una profondità di 8 bit
    struct __attribute__((__packed__)) rgb3_t { uint8_t buf[3]; }; // wrapper to make a more cache friendly code
    static_assert(sizeof(rgb3_t) == 3, "porco il clero");

    const auto bsize = sizeof(rgb3_t) * m_length;
    std::unique_ptr<uint8_t[]> pa = std::make_unique<uint8_t[]>(bsize);
    uint8_t *const beg = pa.get();

    rgb3_t *blk_wr = (rgb3_t *)(void *)beg;
    for (uint16_t r = 0; r < m_height; ++r) {
        for (uint16_t c = 0; c < m_width; ++c) {
            const rgb3_t *px = (const rgb3_t *)(void *)&(this[0](r, c));
            *blk_wr++ = *px;
        }
    }

    // last iteration has move the pointer by sizeof(rgb3_t) -> 3 bytes, which is okay because both are big at least a multiple of 3 bytes
    uint8_t *const end = ((uint8_t*)(void *)blk_wr);
    assert(end <= (beg + bsize));
    assert((size_t)std::distance(beg, end) <= bsize);
    //printf("%p -> len in bytes: %zu, bytes: %zu\n", beg, bsize, std::distance(beg, end));

    auto header = pnm::Header<pnm::Format::PPM6, pnm::BIT_8>{m_width, m_height};
    return ::write_file_content(file_name, header, beg, end), *this;
}