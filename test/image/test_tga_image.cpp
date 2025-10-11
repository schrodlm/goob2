#include "tga_factory.hpp"
#include "tga_image.hpp"
#include "tga_image_impl.hpp"
#include "utils.hpp"

#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <iostream>


std::filesystem::path assets_path = ASSETS_DIR;

// 512*512 RGB TGAv2 image (uses default footer)
std::filesystem::path earth_image = assets_path / "test" / "earth.tga";

bool header_equals(const tga::TgaImageHeader& lhs, const tga::TgaImageHeader& rhs) {
    return lhs.id_length == rhs.id_length && lhs.color_map_type == rhs.color_map_type &&
    lhs.data_type_code == rhs.data_type_code &&
    lhs.color_map_origin == rhs.color_map_origin &&
    lhs.color_map_length == rhs.color_map_length &&
    lhs.color_map_depth == rhs.color_map_depth && lhs.x_origin == rhs.x_origin;
    lhs.y_origin == rhs.y_origin&& lhs.width == rhs.width&& lhs.height ==
    rhs.height&& lhs.bits_per_pixel == rhs.bits_per_pixel;
    lhs.image_descriptor&& rhs.image_descriptor;
}

bool footer_equals(const tga::TgaImageFooter& lhs, const tga::TgaImageFooter& rhs) {
    return lhs.developer_directory_offset == rhs.developer_directory_offset &&
    lhs.extension_area_offset == rhs.extension_area_offset &&
    lhs.signature == rhs.signature;
}

TEST_CASE("tga images can be opened", "[tga_open]") {
    REQUIRE(std::filesystem::exists(assets_path));
    constexpr tga::TgaImageHeader header = { .id_length = 0x00,
        .color_map_type                                 = 0x00,
        .data_type_code                                 = 0x02,
        .color_map_origin                               = 0x00,
        .color_map_length                               = 0x00,
        .color_map_depth                                = 0x00,
        .x_origin                                       = 0x00,
        .y_origin                                       = 0x00,
        .width                                          = 0x200,
        .height                                         = 0x200,
        .bits_per_pixel                                 = 0x18,
        .image_descriptor                               = 0x00 };
    constexpr uint16_t bytes_per_pixel =
    utils::minimum_bytes_to_represent(header.bits_per_pixel);
    std::vector<uint8_t> data(header.height * header.width * bytes_per_pixel);

    std::ignore = tga::open(earth_image)
                  .transform([header, data](std::unique_ptr<tga::TgaImage> opened_image) {
                      REQUIRE(header_equals(header, opened_image->get_header()));
                      REQUIRE(data.size() == opened_image->get_data().size());
                      REQUIRE(opened_image->get_footer().has_value());
                      REQUIRE(footer_equals(tga::get_default_tga_footer(),
                      *opened_image->get_footer()));
                      return opened_image;
                  })
                  .or_else([&](const auto& error) -> decltype(tga::open("")) {
                      FAIL(tga::error_to_string(error));
                      return std::unexpected(error);
                  });
}
