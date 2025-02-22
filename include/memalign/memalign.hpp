#pragma once
#include <cstdlib>
#include <stdexcept>
#include <memory>
#include <new>
#include <type_traits>

#include <memalign/utils.hpp>


#include <iostream>
using std::cout, std::endl;

/* examples
    #include <memalign.hpp>
    struct __attribute__((__packed__)) big_t {
        big_t() {
            p = malloc(113);
            cout << "constructor called, malloc pointer " << p << '\n';
        }
        ~big_t() {
            free(p);
            cout << "destructor called, free pointer " << p << '\n';
        }
        void *p{};
        char buf[11];
    };

    int main() {

        // new (alignas(8) T; -> cpp17+

        malign<int, 8> *p = new malign<int, 8>;
        delete p;

        auto *b = new malign<big_t, 4>;
        cout << b->p << endl;
        delete b;
    }

*/

template <typename T, const size_t ALIGNMENT>
struct malign {
    using aligned_type = T;
    constexpr static auto alignment = ALIGNMENT;

    [[nodiscard]] void * operator new([[maybe_unused]] size_t unused, [[maybe_unused]] std::nothrow_t tag) noexcept {
        constexpr auto aligned_bsize = aligned_bsize_calc<malign<T, ALIGNMENT>::alignment>(sizeof(aligned_type));
        if (aligned_bsize % ALIGNMENT != 0)
            throw std::runtime_error("internal error: incorrectly aligned memory"); // this is a fatal internal library error so result in std::terminate() due to noexcept clause is fine

        cout << "size " << sizeof(aligned_type) << " alignment " << ALIGNMENT << " aligned size " << aligned_bsize << endl;

        if (T *const mem = (T *)std::aligned_alloc(ALIGNMENT, aligned_bsize)) {
            try { return new(mem) T; }
            catch (...) { free(mem); }
        }

        return nullptr;
    }

    [[nodiscard]] void * operator new([[maybe_unused]] size_t unused) noexcept {
        auto *p = new(std::nothrow) malign<T,ALIGNMENT>;
        if (!p) throw std::bad_alloc();
        return p;
    }

    //malign<big_t, 8>::operator delete(ptr);
    void operator delete(void *ptr) noexcept {
        cout << "malign delete" << endl;

        if (!ptr) return;
        if constexpr(std::is_destructible<T>::value) {
            cout << "destructor" << endl;
            ((T *) ptr)->~T();
        }

        free(ptr);
    }

};


/*

template <typenmame T, const size_t ALIGNMENT>
class S: std::allocator<T> {
    T * allocate(size_t n) {

    }
};*/