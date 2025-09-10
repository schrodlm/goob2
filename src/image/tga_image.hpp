#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <vector>

#include "vector.hpp"

enum class RotationDirection {
    horizontal,
    verical,
};

struct RGBColor {
    uint8_t r,g,b,a;

    bool from_code(uint32_t code);
    bool from_array(std::array<uint8_t, 4>);

    uint32_t to_code();
    std::array<uint8_t, 4> to_array();
};

class TgaImage {
public:
    static bool create(std::filesystem::path filepath);
    static bool open(std::filesystem::path filepath);

    void rotate(RotationDirection direction);
    RGBColor get(Vector2<uint32_t>& pos);
    int get_width();
    int get_height();

private:
    TgaImage() = default;
    uint32_t height, width = 0;
    std::vector<uint8_t> data;
};