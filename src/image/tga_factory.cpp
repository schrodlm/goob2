#include "tga_factory.hpp"
#include "tga_image.hpp"
#include "tga_image_impl.hpp"

#include <expected>


#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace tga {
std::expected<std::unique_ptr<TgaImage>, TgaImageError> open(
std::filesystem::path filepath) noexcept {

    if(!std::filesystem::exists(filepath)) {
        return std::unexpected(TgaImageError::BAD_FILE);
    }
    std::ifstream file_stream(filepath.string(), std::ios::binary | std::ios::ate);
    if(!file_stream.is_open()) {
        return std::unexpected(TgaImageError::BAD_FILE);
    }

    auto file_size = file_stream.tellg();
    file_stream.seekg(0, std::ios::beg);

    if(file_size < static_cast<std::streamsize>(sizeof(TgaImageHeader))) {
        return std::unexpected(TgaImageError::HEADER_READ_ERROR);
    }
    // Read header
    TgaImageHeader header;
    file_stream.read(reinterpret_cast<char*>(&header), sizeof(header));
    if(!file_stream || file_stream.gcount() != sizeof(header)) {
        return std::unexpected(TgaImageError::HEADER_READ_ERROR);
    }

    // Check if footer exists
    std::array<uint8_t, TGA_SIGNATURE_SIZE> possible_signature;
    // Seek to 26 bytes before the end of file
    file_stream.seekg(-static_cast<std::streamoff>(TGA_SIGNATURE_SIZE), std::ios::end);
    // Read the last 26 bytes
    file_stream.read(reinterpret_cast<char*>(possible_signature.data()), TGA_SIGNATURE_SIZE);
    bool has_signature = (possible_signature == TGA_SIGNATURE);


    // Seek back to after the header
    file_stream.seekg(sizeof(TgaImageHeader), std::ios::beg);
    auto data_size = file_size - static_cast<std::streamsize>(sizeof(TgaImageHeader));
    if(has_signature) {
        data_size = data_size - static_cast<std::streamsize>(sizeof(TgaImageFooter));
    }
    std::vector<uint8_t> data(data_size);
    file_stream.read(reinterpret_cast<char*>(data.data()), data_size);
    if(!file_stream || file_stream.gcount() != data_size) {
        return std::unexpected(TgaImageError::DATA_READ_ERROR);
    }
    // Initialize footer
    // TODO: Currently ignoring the developer and extension area
    std::optional<TgaImageFooter> footer;
    if(has_signature) {
        TgaImageFooter temp_footer;
        file_stream.read(reinterpret_cast<char*>(&temp_footer), sizeof(temp_footer));
        if(!file_stream || file_stream.gcount() != sizeof(temp_footer)) {
            return std::unexpected(TgaImageError::FOOTER_READ_ERROR);
        }
        footer = temp_footer;
    } else {
        footer = std::nullopt;
    }
    return create(header, data, footer);
}

std::expected<std::unique_ptr<TgaImage>, TgaImageError> create(TgaImageHeader header,
std::span<uint8_t> data,
std::optional<TgaImageFooter> footer /*= get_default_tga_footer()*/) noexcept {
    TgaImageType type = static_cast<TgaImageType>(header.data_type_code);
    switch(type) {
    case TgaImageType::COLOR_MAPPED:
    case TgaImageType::RLE_COLOR_MAPPED:
        if(!verify_header<TgaImageColorMapped>(header)) {
            return std::unexpected(TgaImageError::INVALID_HEADER);
        }
        return std::make_unique<TgaImageColorMapped>(header, data, footer);

    case TgaImageType::RGB:
    case TgaImageType::RLE_RGB:
        if(!verify_header<TgaImageRGB>(header)) {
            return std::unexpected(TgaImageError::INVALID_HEADER);
        }
        return std::make_unique<TgaImageRGB>(header, data, footer);

    case TgaImageType::GRAYSCALE:
    case TgaImageType::RLE_GRAYSCALE:
        if(verify_header<TgaImageGrayscale>(header)) {
            return std::unexpected(TgaImageError::INVALID_HEADER);
        }
        return std::make_unique<TgaImageGrayscale>(header, data, footer);

    default: return std::unexpected(TgaImageError::BAD_TYPE);
    }
}
}; // namespace tga