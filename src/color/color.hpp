#pragma once

#include <cassert>
#include <cstdint>
#include <initializer_list>
#include <stdexcept>

struct RGBAColor;
struct RGBColor {
    uint8_t r, g, b;
    constexpr RGBColor (uint8_t r, uint8_t g, uint8_t b) : r (r), g (g), b (b) {
    }
    constexpr RGBColor (uint32_t code) {
        r = (uint8_t)(code << 24 & 0xFF);
        g = (uint8_t)(code << 16 & 0xFF);
        b = (uint8_t)(code << 8 & 0xFF);
    }

    constexpr RGBColor (std::initializer_list<uint8_t> arr) {
        r = arr.size () >= 1 ? *arr.begin () : 0;
        g = arr.size () >= 2 ? *arr.begin () + 1 : 0;
        b = arr.size () >= 3 ? *arr.begin () + 2 : 0;
    }

    uint32_t to_code () const {
        uint32_t code = 0;
        code += (uint32_t)r << 24;
        code += (uint32_t)g << 16;
        code += (uint32_t)b << 8;

        return code;
    }
    
    // Implicit conversion from RGBAColor
    RGBColor(const RGBAColor& rgba);

    uint8_t operator[] (uint8_t i) const {
        switch (i) {
        case 0: return r;
        case 1: return g;
        case 2: return b;
        default: throw std::out_of_range ("Index out of range");
        }
    }
};

struct RGBAColor {
    uint8_t r, g, b, a;
    constexpr RGBAColor (uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    : r (r), g (g), b (b), a (a) {
    }
    constexpr RGBAColor (uint32_t code) {
        r = (uint8_t)(code << 24 & 0xFF);
        g = (uint8_t)(code << 16 & 0xFF);
        b = (uint8_t)(code << 8 & 0xFF);
        a = (uint8_t)(code << 0 & 0xFF);
    }

    constexpr RGBAColor (std::initializer_list<uint8_t> arr) {
        r = arr.size () >= 1 ? *arr.begin () : 0;
        g = arr.size () >= 2 ? *arr.begin () + 1 : 0;
        b = arr.size () >= 3 ? *arr.begin () + 2 : 0;
        a = arr.size () >= 4 ? *arr.begin () + 3 : 0;
    }

    uint32_t to_code () const {
        uint32_t code = 0;
        code += (uint32_t)r << 24;
        code += (uint32_t)g << 16;
        code += (uint32_t)b << 8;
        code += (uint32_t)a << 0;

        return code;
    }

    RGBAColor(const RGBColor& rgb) : r(rgb.r), g(rgb.g), b(rgb.b), a(255) {}

    uint8_t operator[] (uint8_t i) const {
        switch (i) {
        case 0: return r;
        case 1: return g;
        case 2: return b;
        case 3: return a;
        default: throw std::out_of_range ("Index out of range");
        }
    }
};

// Define the RGBColor constructor that was forward declared
inline RGBColor::RGBColor(const RGBAColor& rgba) : r(rgba.r), g(rgba.g), b(rgba.b) {}

static constexpr RGBAColor TEST = RGBAColor(0,0,0,255);

namespace Colors  {
    constexpr RGBAColor BLACK = RGBAColor(0,0,0,255);
    constexpr RGBAColor WHITE = RGBAColor(255,255,255,255);
    constexpr RGBAColor RED = RGBAColor(255,0,0,255);
    constexpr RGBAColor GREEN = RGBAColor(0,255,0,255);
    constexpr RGBAColor BLUE = RGBAColor(0,0,255,255);
}


