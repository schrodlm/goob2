#include "tga_factory.hpp"
#include "tga_image.hpp"
#include "tga_image_impl.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace tga {
std::expected<std::unique_ptr<TgaImage>, ReadError> open(std::filesystem::path filepath) {

    if(!std::filesystem::exists(filepath)) {
        return std::unexpected(ReadError::BAD_FILE);
    }
    std::ifstream file_stream(filepath.string(), std::ios::binary | std::ios::ate);
    if(!file_stream.is_open()) {
        return std::unexpected(ReadError::BAD_FILE);
    }

    auto file_size = file_stream.tellg();
    file_stream.seekg(0, std::ios::beg);

    if(file_size < static_cast<std::streamsize>(sizeof(TgaImageHeader))) {
        return std::unexpected(ReadError::HEADER_READ_ERROR);
    }
    // Read header
    TgaImageHeader header;
    file_stream.read(reinterpret_cast<char*>(&header), sizeof(header));
    if(!file_stream || file_stream.gcount() != sizeof(header)) {
        return std::unexpected(ReadError::HEADER_READ_ERROR);
    }

    auto data_size = file_size - static_cast<std::streamsize>(sizeof(TgaImageHeader));
    std::vector<uint8_t> data(data_size);
    file_stream.read(reinterpret_cast<char*>(data.data()), data_size);

    if(!file_stream || file_stream.gcount() != data_size) {
        return std::unexpected(ReadError::DATA_READ_ERROR);
    }

    return create(header, data);
}

std::unique_ptr<TgaImage> create(TgaImageHeader header, std::span<uint8_t> data) {
    TgaImageType type = static_cast<TgaImageType>(header.data_type_code);
    switch(type) {
    case TgaImageType::COLOR_MAPPED:
    case TgaImageType::RLE_COLOR_MAPPED:
        return std::make_unique<TgaImageColorMapped>(header, data);

    case TgaImageType::RGB:
    case TgaImageType::RLE_RGB:
        return std::make_unique<TgaImageRGB>(header, data);

    case TgaImageType::GRAYSCALE:
    case TgaImageType::RLE_GRAYSCALE:
        return std::make_unique<TgaImageGrayscale>(header, data);

    default: throw std::invalid_argument("Invalid image type");
    }
}
}; // namespace tga