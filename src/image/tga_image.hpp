#pragma once

#include <cstdint>
#include <filesystem>
#include <vector>
#include <expected>

#include "vector.hpp"
#include "color.hpp"

enum class RotationDirection {
    horizontal,
    verical,
};

#pragma pack(push, 1)
    // Image header is 18 bytes and must not be naturally aligned by the compiler
    struct TgaImageHeader {
        std::uint8_t idlength; // length of a string located after the header
        std::uint8_t colour_map_type; // 0 - image file contains no color map,
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
        std::uint16_t colour_map_origin; // index of first color map entry that
                                         // is included in the file
        std::uint16_t colour_map_length; // number of entries of the color map
                                         // that are included in the file
        std::uint8_t colour_map_depth; // number of bits per color map entry
        std::uint16_t x_origin; // absolute coordinate of lower-left corner for
                                // displays where origin is at the lower left
        std::uint16_t y_origin; // as for X-origin
        std::uint16_t width;    // width in pixels
        std::uint16_t height;   // height in pixels
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

  enum class ReadError {
    BAD_FILE,
    DATA_READ_ERROR,
    HEADER_READ_ERROR,
  };
/*
TGA, often referred to as TARGA, is a raster graphics file format
The format itself permits any pixel bit depth up to 255, of which up to 15 bits
can be dedicated to an alpha channel; however, the only bit depths supported
in practice were 8, 15, 16, 24, and 32, where the 16- and 32-bit formats used 1
and 8 bits respectively for the alpha channel. Color data can be color-mapped,
or in direct color or truecolor format. Image data may be stored raw, or
optionally, a lossless RLE compression similar to PackBits can be employed. This
type of compression performs poorly for typical photographic images, but works
acceptably well for simpler images, such as icons, cartoons and line drawings.
All values are little-endian.

*/

class TgaImage {
    public:
    TgaImage (uint16_t height, uint16_t width, RGBColor color);
    TgaImage (std::vector<uint8_t>& data, TgaImageHeader header);

    static std::expected<TgaImage, ReadError> open (std::filesystem::path filepath);

    void rotate (RotationDirection direction);
    RGBColor get (linalg::uint2& pos);
    bool set (linalg::uint2& pos);

    void write (std::filesystem::path filepath);

    int get_width ();
    int get_height ();

    enum class TgaImageType { COLOR_MAPPED, RGB, GRAYSCALE };

    private:
    
    std::vector<uint8_t> data;
    std::filesystem::path path;
    TgaImageHeader header;
};
