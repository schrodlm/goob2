#pragma once
#include <expected>
#include <span>

#include "tga_image.hpp"

namespace tga {
[[nodiscard]] std::expected<std::unique_ptr<TgaImage>, TgaImageError> open(
std::filesystem::path filepath) noexcept;
[[nodiscard]] std::expected<std::unique_ptr<TgaImage>, TgaImageError>
create(TgaImageHeader header, RGBColor color = Colors::BLACK) noexcept;
[[nodiscard]] std::expected<std::unique_ptr<TgaImage>, TgaImageError>
create(TgaImageHeader header, std::span<uint8_t> data) noexcept;
} // namespace tga