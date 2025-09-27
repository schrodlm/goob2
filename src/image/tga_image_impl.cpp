#include "tga_image_impl.hpp"
#include "tga_image.hpp"
namespace tga {

TgaImageRGB::TgaImageRGB(TgaImageHeader header, std::span<uint8_t> data)
: TgaImage(header, data) {
}
TgaImageRGB::TgaImageRGB(uint32_t width, uint32_t height, RGBColor color /*= Colors::WHITE*/)
: TgaImage(get_default_tga_header<TgaImageRGB>(width, height)) {
    data.assign(width * height * bytes_per_pixel, color.to_code());
}
}; // namespace tga