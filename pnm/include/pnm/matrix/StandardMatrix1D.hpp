#pragma once
#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <memory>
#include <new>

#include <pnm/matrix/AbstractMatrix1D.hpp>

template <typename T, typename Allocator = std::allocator<T>>
class StandardMatrix1D: public AbstractMatrix1D<T> {

    public:
        using allocator_type  = Allocator;
        using allocator_trait = std::allocator_traits<allocator_type>;

    public:
        StandardMatrix1D(uint16_t width, uint16_t height): AbstractMatrix1D<T>{width, height} {

            this->m_vct = (T *)allocator_trait::allocate(m_allocator, this->m_length);
            if constexpr(std::is_constructible<T>::value) {
                for (uint_fast32_t i = 0; i < this->m_length; ++i)
                    allocator_trait::construct(m_allocator, this->m_vct + i);
            }

        }

        ~StandardMatrix1D() {

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

