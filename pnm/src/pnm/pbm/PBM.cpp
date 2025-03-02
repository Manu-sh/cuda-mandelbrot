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
const PBM<pnm::monochrome_t> & PBM<pnm::monochrome_t>::write_file_content_pnm1(const char *const file_name) const {

    // last iteration has move the pointer by sizeof(rgb3_t) -> 3 bytes, which is okay because both are big at least a multiple of 3 bytes
    const uint8_t *const beg = ((uint8_t*)(void *)this->m_vct);
    const uint8_t *const end = ((uint8_t*)(void *)(this->m_vct + this->m_byte_length));
    assert((size_t)std::distance(beg, end) <= this->m_byte_length);
    //printf("%p -> len in bytes: %zu, bytes: %zu\n", beg, this->m_length, std::distance(beg, end));

    auto header = pnm::Header<pnm::Format::PBM4, pnm::BIT_2>{m_width, m_height};
    return ::write_file_content(file_name, header, beg, end), *this;
}
