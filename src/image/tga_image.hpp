#pragma once

#include <cstdint>
#include <expected>
#include <filesystem>
#include <optional>
#include <string>

#include "color.hpp"
#include "vector.hpp"

namespace tga {

// Specifies the version of the TGAImage (New TGA Format if present, Original TGA Format if not)
#define TGA_SIGNATURE_SIZE 18
constexpr std::array<uint8_t, TGA_SIGNATURE_SIZE> TGA_SIGNATURE = { 'T', 'R', 'U',
    'E', 'V', 'I', 'S', 'I', 'O', 'N', '-', 'X', 'F', 'I', 'L', 'E', '.', 0 };

enum class RotationDirection {
    horizontal,
    vertical,
};

enum class TgaImageError {
#define DEFINE_ERROR(name, value) name = value,

// include the list to expand the macro for each error
#include "error_list.txt"

// undefine the macro to prevent it from leaking
#undef DEFINE_ERROR
};

std::string error_to_string(TgaImageError);

enum class TgaImageType : uint8_t {
    COLOR_MAPPED     = 0b0001,                // 1
    RGB              = 0b0010,                // 2
    GRAYSCALE        = 0b0011,                // 3
    RLE_COLOR_MAPPED = COLOR_MAPPED | 0b1000, // 9
    RLE_RGB          = RGB | 0b1000,          // 10
    RLE_GRAYSCALE    = GRAYSCALE | 0b1000,    // 11
    _INVALID
};

#pragma pack(push, 1)
// Image header is 18 bytes and must not be naturally aligned by the compiler
struct TgaImageHeader {
    std::uint8_t id_length;      // length of a string located after the header
    std::uint8_t color_map_type; // 0 - image file contains no color map,
                                 // 1 - color map present
    /*
    0 - no image data is present
    1 - uncompressed color-mapped image
    2 - uncompressed true-color image
    3 - uncompressed grayscale image
    9 - run-length encoded color-mapped image
    10 - run-length encoded true-color image
    11 - run-length encoded grayscale image
    */
    std::uint8_t data_type_code;
    std::uint16_t color_map_origin; // index of first color map entry that
                                    // is included in the file
    std::uint16_t color_map_length; // number of entries of the color map
                                    // that are included in the file
    std::uint8_t color_map_depth;   // number of bits per color map entry
    std::uint16_t x_origin;      // absolute coordinate of lower-left corner for
                                 // displays where origin is at the lower left
    std::uint16_t y_origin;      // as for X-origin
    std::uint16_t width;         // width in pixels
    std::uint16_t height;        // height in pixels
    std::uint8_t bits_per_pixel; // pixel depth
    /*
    bits 3–0 - the alpha channel depth
    bits 5–4 - pixel ordering
    bit 4 of the image descriptor byte indicates right-to-left pixel
    ordering if set. Bit 5 indicates an ordering of top-to-bottom Otherwise,
    pixels are stored in bottom-to-top, left-to-right order.
    */
    std::uint8_t image_descriptor;
};
#pragma pack(pop)
static_assert(sizeof(TgaImageHeader) == 18);

/*
Bytes 0-3: The Extension Area Offset
Bytes 4-7: The Developer Directory Offset
Bytes 8-23: The Signature
Byte 24: ASCII Character "."
Byte 25: Binary zero string terminator (0x00)

Note: A TGA Reader should begin by determining whether the desired file is in the Original TGA Format or the New TGA Format.
This is accomplished by examining the last 26 bytes of the file.
To determine whether the acquired data constitutes a legal TGA File Footer,
scan bytes 8-23 of the footer as ASCII characters and determine whether they match the signature string: TRUEVISION-XFILE
*/
#pragma pack(push, 1)
struct TgaImageFooter {
    uint32_t extension_area_offset;
    uint32_t developer_directory_offset;
    std::array<uint8_t, TGA_SIGNATURE_SIZE> signature;
};
#pragma pack(pop)

static_assert(sizeof(TgaImageFooter) == 26);

class TgaImage {
    public:
    void rotate(RotationDirection direction);
    RGBColor get(linalg::uint2& pos) const;
    bool set(linalg::uint2& pos);

    void write(std::filesystem::path filepath);

    uint16_t get_width() const {
        return header.width;
    }
    uint16_t get_height() const {
        return header.height;
    }
    uint16_t get_bytes_per_pixel() const {
        return bytes_per_pixel;
    }
    const TgaImageHeader& get_header() const {
        return header;
    }
    const std::optional<TgaImageFooter>& get_footer() const {
        return footer;
    }
    const std::span<const uint8_t> get_data() const {
        return data;
    }

    virtual RGBColor operator[](linalg::uint2& pos) {
        if(pos.x >= header.width || pos.y >= header.height)
            throw std::out_of_range("...");
        return RGBColor(data[0]);
    }

    virtual ~TgaImage() = default;

    protected:
    TgaImage(uint16_t height, uint16_t width, uint8_t bits_per_pixel, std::optional<TgaImageFooter> footer);
    TgaImage(TgaImageHeader header, std::span<uint8_t>& data, std::optional<TgaImageFooter> footer);
    TgaImage(TgaImageHeader header, std::optional<TgaImageFooter> footer);

    TgaImageHeader header;
    // not present when working Original TGA Format
    std::optional<TgaImageFooter> footer;
    // TODO: Add developer area data
    // TODO: Add extension area data
    std::vector<uint8_t> data;
    uint16_t bytes_per_pixel;
};


}; // namespace tga
