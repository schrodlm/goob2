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

    RGBColor get(linalg::uint2& pos) const;
};
}; // namespace tga