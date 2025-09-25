#pragma once
#include <expected>
#include <span>

#include "tga_image.hpp"

namespace tga {
std::expected<std::unique_ptr<TgaImage>, ReadError> open(std::filesystem::path filepath);
std::unique_ptr<TgaImage> create(TgaImageHeader header, RGBColor color = Colors::BLACK);
std::unique_ptr<TgaImage> create(TgaImageHeader header, std::span<uint8_t> data);
} // namespace tga