#pragma once
#include <cstdint>
#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <memory>
#include <new>

#include <pnm/memalign/utils.hpp>
#include <pnm/memalign/AlignedAllocator.hpp>
#include <pnm/matrix/AbstractMatrix1D.hpp>

// memory aligned to 256 bit (32 byte) to use avx2
template <typename T, typename Allocator = AlignedAllocator<T, sizeof(T)> >
class AlignedMatrix1D: public AbstractMatrix1D<T> {

    public:
        using allocator_type  = Allocator;
        using allocator_trait = std::allocator_traits<allocator_type>;
        static constexpr size_t cell_alignment = allocator_type::alignment; // A static data member declared constexpr on its first declaration is implicitly an inline variable.

    public:
        explicit AlignedMatrix1D(uint16_t width, uint16_t height);

        ~AlignedMatrix1D() {

            if constexpr(std::is_destructible<T>::value) {
                for (uint_fast32_t i = 0; i < this->m_length; ++i)
                    allocator_trait::destroy(m_allocator, this->m_vct + i);
            }

            allocator_trait::deallocate(m_allocator, this->m_vct, this->m_length);
            this->m_vct = (T *)(long)(this->m_length = this->m_width = this->m_height = 0);
        }


    protected:
        allocator_type m_allocator;
};

template <typename T, typename Allocator>
AlignedMatrix1D<T, Allocator>::AlignedMatrix1D(uint16_t width, uint16_t height)
        : AbstractMatrix1D<T>{width, height} {

/*
    m_vct = (T *)__builtin_assume_aligned(
            new (std::align_val_t(ALIGNMENT)) T[aligned_bsize_calc<ALIGNMENT>(
                    aligned_bsize_calc<ALIGNMENT>(width) * height // align every row
            )],
            ALIGNMENT
    );
*/
    this->m_vct = (T *)allocator_trait::allocate(m_allocator, this->m_length);
    if constexpr(std::is_constructible<T>::value) {
        for (uint_fast32_t i = 0; i < this->m_length; ++i)
            allocator_trait::construct(m_allocator, this->m_vct + i);
    }

    // passing a size in bytes which is not a multiple of ALIGNMENT result in UB
    // for example aligned_alloc(32, 65) and aligned_alloc(32, 63) are UB
    // while aligned_alloc(32, 64) is correct
    if (((long)this->m_vct) % cell_alignment != 0)
        throw std::runtime_error("incorrectly aligned memory");
}
