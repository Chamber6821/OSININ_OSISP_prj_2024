#pragma once

#include <cstdint>

constexpr std::uint8_t mergeBytes(std::uint8_t byte) { return byte; }

constexpr std::uint16_t mergeBytes(std::uint8_t byte1, std::uint8_t byte2) {
  return static_cast<std::uint16_t>(byte1) << 8 |
         static_cast<std::uint16_t>(byte2);
}

constexpr std::uint32_t mergeBytes(
  std::uint8_t byte1, std::uint8_t byte2, std::uint8_t byte3, std::uint8_t byte4
) {
  return static_cast<std::uint32_t>(byte1) << 24 |
         static_cast<std::uint32_t>(byte2) << 16 |
         static_cast<std::uint32_t>(byte3) << 8 |
         static_cast<std::uint32_t>(byte4);
}
