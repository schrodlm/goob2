#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <expected>

#include "tga_image.hpp"

TgaImage::TgaImage(uint16_t height, uint16_t width, RGBColor color) {
  header = {
    .width = width,
    .height = height
  };
  memset(data.data(), color.to_code(), height*width);
}

std::expected<TgaImage, ReadError> TgaImage::open(std::filesystem::path filepath){
      
    if(!std::filesystem::exists(filepath)) {
      return std::unexpected(ReadError::BAD_FILE);
    }
    std::ifstream file_stream(filepath.string(), std::ios::binary|std::ios::ate);
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
    if (!file_stream || file_stream.gcount() != sizeof(header)) {
      return std::unexpected(ReadError::HEADER_READ_ERROR);
    }
    
    auto data_size = file_size - static_cast<std::streamsize>(sizeof(TgaImageHeader));
    std::vector<uint8_t> data(data_size);   
    file_stream.read(reinterpret_cast<char*>(data.data()),data_size);

    if (!file_stream || file_stream.gcount() != data_size) {
        return std::unexpected(ReadError::DATA_READ_ERROR);
    }

    return TgaImage(data, header);
}
