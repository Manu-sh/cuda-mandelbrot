#pragma once
#include <string_view>
#include <sstream>
#include <string>
#include <cstdint>

#include <pnm/pixels/depth.hpp>

namespace pnm {

    enum class Format: uint8_t {
        PPM3, PPM6,
        PGM2, PGM5,
        PBM4, PBM1
    };

    // ChannelDepth<BIT_8>::max_value() -> ChannelDepth<8>::max_value()
    template <const uint8_t T> struct ChannelDepth;

    template<>
    struct ChannelDepth<pnm::BIT_8> {
        struct max_value {
            constexpr operator std::string_view() const noexcept { return "255"; }
        };
    };

    template<>
    struct ChannelDepth<pnm::BIT_2> {
        struct max_value {
            constexpr operator std::string_view() const noexcept { return ""; }
        };
    };

    template <const pnm::Format F, const uint8_t BIT_DEPTH>
    struct AbstractHeader {

        protected:
            AbstractHeader(uint16_t width, uint16_t height, std::string_view type)
                    : m_type{type}, m_width{width}, m_height{height} {}

        public:
            operator std::string() {
                using depth_t = typename ChannelDepth<BIT_DEPTH>::max_value; // for some reason Channel<BIT_DEPTH>max_value{} doesnt compile
                std::ostringstream ss;

                ss << this->m_type << '\n'                                                            // pnm type
                   << std::to_string(this->m_width) << ' ' << std::to_string(this->m_height) << '\n'; // width x height

               if (std::string_view max_value = depth_t{}; max_value.length())
                   ss << max_value << '\n';                                                           // MAX_VALUE, end of header, not needed for PBM

                return ss.str();
            }

        protected:
            const std::string_view m_type;
            const uint16_t m_width, m_height;
    };

    template <const pnm::Format, const uint8_t BIT_DEPTH> struct Header; // Forward Decl

    template<const uint8_t BIT_DEPTH>
    struct Header<pnm::Format::PPM3, BIT_DEPTH>: AbstractHeader<pnm::Format::PPM3, BIT_DEPTH> {
        Header(uint16_t width, uint16_t height)
                : AbstractHeader<pnm::Format::PPM3, BIT_DEPTH>{width, height, "P3"} {}
    };

    template<const uint8_t BIT_DEPTH>
    struct Header<pnm::Format::PPM6, BIT_DEPTH>: AbstractHeader<pnm::Format::PPM6, BIT_DEPTH> {
        Header(uint16_t width, uint16_t height)
                : AbstractHeader<pnm::Format::PPM6, BIT_DEPTH>{width, height, "P6"} {}
    };


    template<const uint8_t BIT_DEPTH>
    struct Header<pnm::Format::PGM2, BIT_DEPTH>: AbstractHeader<pnm::Format::PGM2, BIT_DEPTH> {
        Header(uint16_t width, uint16_t height)
                : AbstractHeader<pnm::Format::PGM2, BIT_DEPTH>{width, height, "P2"} {}
    };

    template<const uint8_t BIT_DEPTH>
    struct Header<pnm::Format::PGM5, BIT_DEPTH>: AbstractHeader<pnm::Format::PGM5, BIT_DEPTH> {
        Header(uint16_t width, uint16_t height)
                : AbstractHeader<pnm::Format::PGM5, BIT_DEPTH>{width, height, "P5"} {}
    };


    template<>
    struct Header<pnm::Format::PBM4, pnm::BIT_2>: AbstractHeader<pnm::Format::PBM4, pnm::BIT_2> {
        Header(uint16_t width, uint16_t height)
                : AbstractHeader<pnm::Format::PBM4, pnm::BIT_2>{width, height, "P4"} {}
    };


    template<>
    struct Header<pnm::Format::PBM1, pnm::BIT_2>: AbstractHeader<pnm::Format::PBM1, pnm::BIT_2> {
        Header(uint16_t width, uint16_t height)
                : AbstractHeader<pnm::Format::PBM1, pnm::BIT_2>{width, height, "P1"} {}
    };


}