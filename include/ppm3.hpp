#pragma once
#include <cstdint>
#include <cassert>
#include <cinttypes>
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>

#include <common.hpp>
#include <Matrix1D.hpp>

// https://en.wikipedia.org/wiki/Netpbm

struct PPM3: public Matrix1D<rgb_t> {

    using pixel_type = typename PPM3::Matrix1D<rgb_t>::matrix_type;

    PPM3(const PPM3 &o) = delete;
    PPM3 & operator=(const PPM3 &o) = delete;

    explicit PPM3(uint16_t width, uint16_t height)
        : Matrix1D<rgb_t>{height, width} {
    }

    inline rgb_t * unwrap() { return Matrix1D<rgb_t>::unwrap(); }
    inline uint16_t height() const noexcept { return Matrix1D<rgb_t>::height();  }
    inline uint16_t width()  const noexcept { return Matrix1D<rgb_t>::width();   }

    const PPM3 & write_file_content(const char *file_name) const {

        static const char *const map[256] = {
             "0  ",  "1  ",   "2 ",   "3 ",   "4 ",   "5 ",   "6 ",   "7 ",   "8 ",   "9 ",   "10 ",  "11 ",  "12 ",  "13 ",  "14 ",  "15 ",
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

#if 1
        std::ostringstream ss;

        // header
        ss << m_type[0] << m_type[1] << '\n' // ppm type
           << std::to_string(m_width) << ' ' << std::to_string(m_height) << '\n' // width x height
           << "255\n";                                                           // end of header

        std::string s{ss.str()};
        s.reserve(s.length() + (m_length * strlen("255 ")) + 50);

        for (uint16_t r = 0; r < m_height; ++r) {
            for (uint16_t c = 0; c < m_width; ++c) {
                const rgb_t px = this->operator()(r, c);
                s += map[px.r];
                s += map[px.b];
                s += map[px.g];
            }
        }

        using std::ios_base;

        std::ofstream fppm3;
        fppm3.exceptions(ios_base::failbit|ios_base::badbit);
        fppm3.open(file_name, ios_base::out|ios_base::binary|ios_base::trunc);
        fppm3.write(s.c_str(), s.length());

#else
        std::ostringstream ss;

        // header
        ss << m_type[0] << m_type[1] << '\n' // ppm type
           << std::to_string(m_width) << ' ' << std::to_string(m_height) << '\n' // width x height
           << "255\n";                                                           // end of header

        for (uint16_t r = 0; r < m_height; ++r) {
            for (uint16_t c = 0; c < m_width; ++c) {
                const rgb_t px = this->operator()(r, c);
                ss << map[px.r]
                   << map[px.b]
                   << map[px.g] << "\n";
            }
        }

        const auto s = ss.str();

        using std::ios_base;

        std::ofstream fppm3;
        fppm3.exceptions(ios_base::failbit|ios_base::badbit);
        fppm3.open(file_name, ios_base::out|ios_base::binary|ios_base::trunc);
        fppm3.write(s.c_str(), s.length());
        //fppm3 << s;
#endif
        return *this;
    }

    private:
        static const uint8_t m_type[2];
};

const uint8_t PPM3::m_type[2] = {'P', '3'};
