#pragma once

#include <cstdint>
constexpr uint8_t BYTE_SIZE = 8;

namespace utils {
constexpr inline uint32_t minimum_bytes_to_represent(const uint32_t n) {
    return ((n + BYTE_SIZE - 1) >> 3);
}
}; // namespace utils