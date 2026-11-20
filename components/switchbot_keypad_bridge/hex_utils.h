#pragma once

// ESPHome 2026.11 removed the std::string-returning esphome::format_hex_pretty().
// This one lives in our namespace, so it shadows the ESPHome version on older
// releases and replaces it on newer ones — call sites stay unchanged.

#include <cstddef>
#include <cstdint>
#include <string>

namespace esphome {
namespace switchbot_keypad_bridge {

inline std::string format_hex_pretty(const uint8_t *data, size_t length) {
  static const char HEX_CHARS[] = "0123456789ABCDEF";
  std::string out;
  if (data == nullptr || length == 0)
    return out;
  out.reserve(length * 3 + 8);
  for (size_t i = 0; i < length; i++) {
    if (i > 0)
      out += '.';
    out += HEX_CHARS[data[i] >> 4];
    out += HEX_CHARS[data[i] & 0x0F];
  }
  out += " (" + std::to_string(length) + ")";
  return out;
}

}  // namespace switchbot_keypad_bridge
}  // namespace esphome
