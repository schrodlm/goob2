#pragma once

#include <cstdint>
#include <span>

#include "color.hpp"
#include "tga_image.hpp"
#include "vector.hpp"


namespace tga {
class TgaImageColorMapped : public TgaImage {
    public:
    TgaImageColorMapped(TgaImageHeader header, std::span<uint8_t> data)
    : TgaImage(header, data) {};

    RGBColor get(linalg::uint2& pos) const;
};

class TgaImageRGB : public TgaImage {
    public:
    TgaImageRGB(TgaImageHeader header, std::span<uint8_t> data)
    : TgaImage(header, data) {};
    RGBColor get(linalg::uint2& pos) const;
};

class TgaImageGrayscale : public TgaImage {
    public:
    TgaImageGrayscale(TgaImageHeader header, std::span<uint8_t> data)
    : TgaImage(header, data) {};
};
template <typename T>
concept IsTgaImageOrDerived = std::is_base_of_v<TgaImage, T>;

template <IsTgaImageOrDerived T>
constexpr TgaImageHeader
get_default_tga_header(uint16_t width, uint16_t height, bool rle_encoded = false) {
    if constexpr(std::is_same_v<T, TgaImageColorMapped>) {
        TgaImageHeader header{};
        header.color_map_type   = 1;
        header.data_type_code   = rle_encoded ?
          std::to_underlying(TgaImageType::RLE_COLOR_MAPPED) :
          std::to_underlying(TgaImageType::COLOR_MAPPED);
        header.color_map_length = 256; // standard 256-color image
        header.color_map_depth =
        24; // each color is represented by 3 bytes - 1B red, 1B green, 1B blue
        header.width  = width;
        header.height = height;
        header.bits_per_pixel =
        8; // each pixel is identified by an index to the color map (log2(256))
        return header;

    } else if constexpr(std::is_same_v<T, TgaImageRGB>) {
        TgaImageHeader header{};
        header.data_type_code = rle_encoded ?
        std::to_underlying(TgaImageType::RLE_RGB) :
        std::to_underlying(TgaImageType::RGB);
        header.width          = width;
        header.height         = height;
        header.bits_per_pixel =
        24; // each pixel is defined by 3 bytes of data - 1B red, 1B green, 1B blue
        return header;
    } else if constexpr(std::is_same_v<T, TgaImageGrayscale>) {
        TgaImageHeader header;
        header.data_type_code = rle_encoded ?
        std::to_underlying(TgaImageType::RLE_GRAYSCALE) :
        std::to_underlying(TgaImageType::GRAYSCALE);
        header.width          = width;
        header.height         = height;
        header.bits_per_pixel =
        8; // each pixel is defined by 1 byte - specifying the shade of gray (256 shades of gray haha)
        return header;
    } else {
        static_assert(std::is_void_v<T> && !std::is_void_v<T>, "Unsupported TGA image type");
    }
}

template <IsTgaImageOrDerived T>
constexpr bool verify_header(TgaImageHeader& header) {
    if constexpr(std::is_same_v<T, TgaImageColorMapped>) {
        if((header.data_type_code != std::to_underlying(TgaImageType::COLOR_MAPPED) &&
           header.data_type_code != std::to_underlying(TgaImageType::RLE_COLOR_MAPPED))) {
            return false;
        } else if(header.color_map_type != 1) {
            return false;
        } else {
            return true;
        }
    } else if constexpr(std::is_same_v<T, TgaImageRGB>) {
        if((header.data_type_code != std::to_underlying(TgaImageType::RGB) &&
           header.data_type_code != std::to_underlying(TgaImageType::RLE_RGB))) {
            return false;
        } else if(header.color_map_type == 1) {
            return false;
        } else {
            return true;
        }
    } else if constexpr(std::is_same_v<T, TgaImageGrayscale>) {
        if((header.data_type_code != std::to_underlying(TgaImageType::GRAYSCALE) &&
           header.data_type_code != std::to_underlying(TgaImageType::RLE_GRAYSCALE))) {
            return false;
        } else if(header.color_map_type == 1) {
            return false;
        } else {
            return true;
        }
    } else {
        static_assert(std::is_void_v<T> && !std::is_void_v<T>, "Unsupported TGA image type");
    }
}
}; // namespace tga