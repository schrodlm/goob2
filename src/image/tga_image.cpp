#include <cstdint>
#include <cstring>
#include <expected>


#include "tga_image.hpp"

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

TgaImage::TgaImage(uint16_t height, uint16_t width, RGBColor color) {
    header.width  = width;
    header.height = height;
    memset(data.data(), color.to_code(), height * width);
}

TgaImage::TgaImage(TgaImageHeader header, std::span<uint8_t>& data_in)
: header(header) {
    data.assign(data_in.begin(), data_in.end());
}

TgaImage::TgaImage(TgaImageHeader header, RGBColor color /*= Colors::BLACK*/) : header(header) {
    data.assign(header.width * header.height, color.to_code());
}


} // namespace tga