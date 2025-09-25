#include <cstdint>
#include <cstring>
#include <expected>


#include "tga_image.hpp"

using namespace tga;

TgaImage::TgaImage(uint16_t height, uint16_t width, RGBColor color) {
    header.width  = width;
    header.height = height;
    memset(data.data(), color.to_code(), height * width);
}


