#pragma once
#include <cstdint>
#include <cstring>
#include <sstream>
#include <string>
#include <fstream>
#include <memory>
#include <iterator>

#include <common.hpp>
#include <pixels.hpp>
#include <Matrix1D.hpp>

using pixels::rgb_t;

// https://en.wikipedia.org/wiki/Netpbm
struct PPM: public Matrix1D<rgb_t> {

    enum class Format: uint8_t {
        PPM3, PPM6
    };

    static constexpr auto pixel_type_alignment = PPM::Matrix1D<rgb_t>::matrix_cell_alignment; // sizeof(rgb_t) == alignof(rgb_t)
    using pixel_type = typename PPM::Matrix1D<rgb_t>::matrix_cell_type; // simply rgb_t

    PPM(const PPM &o) = delete;
    PPM & operator=(const PPM &o) = delete;

    explicit PPM(uint16_t width, uint16_t height)
        : Matrix1D<rgb_t>{height, width} {
    }

    template <Format format>
    constexpr FORCED(inline) const PPM & write_file_content(const char *const file_name) const {
        if constexpr(format == Format::PPM3)
            return write_file_content_ppm3(file_name);
        return write_file_content_ppm6(file_name);
    }

    private:
        const PPM & write_file_content_ppm3(const char *const file_name) const;
        const PPM & write_file_content_ppm6(const char *const file_name) const;
};



const PPM & PPM::write_file_content_ppm3(const char *const file_name) const {

    static constexpr const char *const map[256] = { // string_view perform worse
        "0 ",  "1 ",    "2 ",   "3 ",    "4 ",   "5 ",   "6 ",   "7 ",   "8 ",   "9 ",  "10 ",  "11 ",  "12 ",  "13 ",  "14 ",  "15 ",
        "16 ",  "17 ",  "18 ",  "19 ",   "20 ",  "21 ",  "22 ",  "23 ",  "24 ",  "25 ",  "26 ",  "27 ",  "28 ",  "29 ",  "30 ",  "31 ",
        "32 ",  "33 ",  "34 ",  "35 ",   "36 ",  "37 ",  "38 ",  "39 ",  "40 ",  "41 ",  "42 ",  "43 ",  "44 ",  "45 ",  "46 ",  "47 ",
        "48 ",  "49 ",  "50 ",  "51 ",   "52 ",  "53 ",  "54 ",  "55 ",  "56 ",  "57 ",  "58 ",  "59 ",  "60 ",  "61 ",  "62 ",  "63 ",
        "64 ",  "65 ",  "66 ",  "67 ",   "68 ",  "69 ",  "70 ",  "71 ",  "72 ",  "73 ",  "74 ",  "75 ",  "76 ",  "77 ",  "78 ",  "79 ",
        "80 ",  "81 ",  "82 ",  "83 ",   "84 ",  "85 ",  "86 ",  "87 ",  "88 ",  "89 ",  "90 ",  "91 ",  "92 ",  "93 ",  "94 ",  "95 ",
        "96 ",  "97 ",  "98 ",  "99 ",  "100 ", "101 ", "102 ", "103 ", "104 ", "105 ", "106 ", "107 ", "108 ", "109 ", "110 ", "111 ",
        "112 ", "113 ", "114 ", "115 ",  "116 ", "117 ", "118 ", "119 ", "120 ", "121 ", "122 ", "123 ", "124 ", "125 ", "126 ", "127 ",
        "128 ", "129 ", "130 ", "131 ",  "132 ", "133 ", "134 ", "135 ", "136 ", "137 ", "138 ", "139 ", "140 ", "141 ", "142 ", "143 ",
        "144 ", "145 ", "146 ", "147 ",  "148 ", "149 ", "150 ", "151 ", "152 ", "153 ", "154 ", "155 ", "156 ", "157 ", "158 ", "159 ",
        "160 ", "161 ", "162 ", "163 ",  "164 ", "165 ", "166 ", "167 ", "168 ", "169 ", "170 ", "171 ", "172 ", "173 ", "174 ", "175 ",
        "176 ", "177 ", "178 ", "179 ",  "180 ", "181 ", "182 ", "183 ", "184 ", "185 ", "186 ", "187 ", "188 ", "189 ", "190 ", "191 ",
        "192 ", "193 ", "194 ", "195 ",  "196 ", "197 ", "198 ", "199 ", "200 ", "201 ", "202 ", "203 ", "204 ", "205 ", "206 ", "207 ",
        "208 ", "209 ", "210 ", "211 ",  "212 ", "213 ", "214 ", "215 ", "216 ", "217 ", "218 ", "219 ", "220 ", "221 ", "222 ", "223 ",
        "224 ", "225 ", "226 ", "227 ",  "228 ", "229 ", "230 ", "231 ", "232 ", "233 ", "234 ", "235 ", "236 ", "237 ", "238 ", "239 ",
        "240 ", "241 ", "242 ", "243 ",  "244 ", "245 ", "246 ", "247 ", "248 ", "249 ", "250 ", "251 ", "252 ", "253 ", "254 ", "255 "
    };

    static constexpr const uint8_t map_length[256] = {
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4
    };

    std::ostringstream ss;

    // header
    ss << "P3\n" // ppm type
       << std::to_string(m_width) << ' ' << std::to_string(m_height) << '\n' // width x height
       << "255\n";                                                           // end of header


    // +1 for null terminator we actually dont use but we need *p
    // to be writeable to make use of std::distance(mem, last)
#if 1
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
            memcpy(p, map[px.r], map_length[px.r]), p += map_length[px.r];
            memcpy(p, map[px.b], map_length[px.b]), p += map_length[px.b];
            memcpy(p, map[px.g], map_length[px.g]), p += map_length[px.g];
        }
    }

    assert(p <= &mem[bsize-1]);
    assert(std::distance(mem, p) <= bsize-1);
    *p = '\0'; // p is writable here

    const auto s = ss.str();
    using std::ios_base;

    std::ofstream fPPM;
    fPPM.exceptions(ios_base::failbit|ios_base::badbit);
    fPPM.open(file_name, ios_base::out|ios_base::binary|ios_base::trunc);
    fPPM.write(s.data(), s.length());
    fPPM.write(mem, std::distance(mem, p));

    return *this;
}


const PPM & PPM::write_file_content_ppm6(const char *const file_name) const {

    std::ostringstream ss;

    // header
    ss << "P6\n" // ppm type
       << std::to_string(m_width) << ' ' << std::to_string(m_height) << '\n' // width x height
       << "255\n";

    struct __attribute__((__packed__)) rgb3_t { uint8_t buf[3]; }; // wrapper to make a more cache friendly code
    static_assert(sizeof(rgb3_t) == 3, "porco il clero");

    // we add an extra element of size(rgb3_t) so at the end of the loop p still writeable (we move with 3 bytes every iteration instead of 1)
    // so at the end of the loop adding extra 3 bytes we can write on *p technically 3 bytes but we dont use it, it's just for clarity
    // in fact using std::distance() without fixing the address pointed by we will occur in wrong size and if we use that size... well you should know
    const auto bsize = sizeof(rgb3_t) * m_length;
    std::unique_ptr<uint8_t[]> pa = std::make_unique<uint8_t[]>(bsize);
    uint8_t *const beg = pa.get();

    rgb3_t *blk = (rgb3_t *)(void *)beg;

    for (uint16_t r = 0; r < m_height; ++r) {
        for (uint16_t c = 0; c < m_width; ++c) {
            const rgb3_t *px = (const rgb3_t *)(void *)&(this->operator()(r, c));
            *blk = *px;
            ++blk;
        }
    }

    // last iteration has move the pointer by sizeof(rgb3_t) -> 3 bytes, which is okay because both are big at least a multiple of 3 bytes
    uint8_t *const end = ((uint8_t*)(void *)blk);
    assert(end <= (beg + bsize));
    assert(std::distance(beg, end) <= bsize);
    //printf("%p -> len in bytes: %zu, bytes: %zu\n", beg, bsize, std::distance(beg, end));

    const auto s = ss.str();
    using std::ios_base;

    std::ofstream fPPM;
    fPPM.exceptions(ios_base::failbit|ios_base::badbit);
    fPPM.open(file_name, ios_base::out|ios_base::binary|ios_base::trunc);
    fPPM.write(s.data(), s.length());
    fPPM.write((char *)beg, std::distance(beg, end));

    return *this;

}