#include <cstdint>
#include <cstring>
#include <expected>


#include "tga_image.hpp"
#include "utils.hpp"

namespace tga {

std::string error_to_string(TgaImageError error) {
    switch(error) {
        #define DEFINE_ERROR(name, value)   \
            case TgaImageError::name:       \
                return #name; 
        #include "error_list.txt"
#undef DEFINE_ERROR
    }
}

TgaImage::TgaImage(uint16_t height, uint16_t width, uint8_t bits_per_pixel) {
    header.width  = width;
    header.height = height;
    header.bits_per_pixel = bits_per_pixel;
    bytes_per_pixel       = utils::minimum_bytes_to_represent(bits_per_pixel);
    memset(data.data(), 0, height * width * bytes_per_pixel);
}

TgaImage::TgaImage(TgaImageHeader header, std::span<uint8_t>& data_in)
: header(header) {
    data.assign(data_in.begin(), data_in.end());
}

TgaImage::TgaImage(TgaImageHeader header, RGBColor color /*= Colors::BLACK*/) : header(header) {
    data.assign(header.width * header.height, color.to_code());
}


} // namespace tga