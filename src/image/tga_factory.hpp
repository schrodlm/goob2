#pragma once
#include <expected>
#include <span>

#include "tga_image.hpp"
#include "tga_image_impl.hpp"

namespace tga {
[[nodiscard]] std::expected<std::unique_ptr<TgaImage>, TgaImageError> open(
std::filesystem::path filepath) noexcept;
[[nodiscard]] std::expected<std::unique_ptr<TgaImage>, TgaImageError> create(TgaImageHeader header,
RGBColor color                       = Colors::BLACK,
std::optional<TgaImageFooter> footer = get_default_tga_footer()) noexcept;
[[nodiscard]] std::expected<std::unique_ptr<TgaImage>, TgaImageError> create(TgaImageHeader header,
std::span<uint8_t> data,
std::optional<TgaImageFooter> footer = get_default_tga_footer()) noexcept;
} // namespace tga