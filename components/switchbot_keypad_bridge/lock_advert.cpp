#include "lock_advert.h"

namespace esphome {
namespace switchbot_keypad_bridge {

namespace {

int valid_percent(uint8_t raw) {
  const int value = raw & 0x7F;
  return value <= 100 ? value : -1;
}

}  // namespace

int parse_lock_battery(PhysicalLockModel model,
                       const uint8_t *service_data, size_t service_len,
                       const uint8_t *manufacturer_payload,
                       size_t manufacturer_len) {
  switch (model) {
    case PhysicalLockModel::LOCK:
    case PhysicalLockModel::LOCK_LITE:
    case PhysicalLockModel::LOCK_VISION:
      if (service_data == nullptr || service_len < 3) return -1;
      return valid_percent(service_data[2]);

    case PhysicalLockModel::LOCK_PRO:
    case PhysicalLockModel::LOCK_PRO_WIFI:
    case PhysicalLockModel::LOCK_ULTRA:
    case PhysicalLockModel::LOCK_VISION_PRO:
      if (manufacturer_payload == nullptr || manufacturer_len < 10) return -1;
      return valid_percent(manufacturer_payload[9]);

    default:
      return -1;
  }
}

LockAdvertState parse_lock_state(PhysicalLockModel model,
                                 const uint8_t *m, size_t len) {
  LockAdvertState out;
  if (m == nullptr) return out;
  switch (model) {
    case PhysicalLockModel::LOCK:  // process_wolock
      if (len < 9) return out;
      out.status = (m[7] & 0x70) >> 4;
      out.door_open = (m[7] & 0x04) ? 1 : 0;
      return out;

    case PhysicalLockModel::LOCK_LITE:  // process_locklite: no door sensor
    case PhysicalLockModel::LOCK_VISION:
      if (len < 9) return out;
      out.status = (m[7] & 0x70) >> 4;
      return out;

    case PhysicalLockModel::LOCK_PRO:  // parse_common_data (Pro, Ultra, Vision Pro)
    case PhysicalLockModel::LOCK_PRO_WIFI:
    case PhysicalLockModel::LOCK_ULTRA:
    case PhysicalLockModel::LOCK_VISION_PRO:
      if (len < 12) return out;
      out.status = (m[7] & 0x78) >> 3;
      out.door_open = (m[8] & 0x10) ? 1 : 0;
      return out;

    default:
      return out;
  }
}

const char *lock_status_str(int status) {
  static const char *const NAMES[] = {
      "Locked",          "Unlocked",          "Locking",      "Unlocking",
      "Locking blocked", "Unlocking blocked", "Latch locked", "Half locked",
  };
  return (status >= 0 && status < 8) ? NAMES[status] : "Unknown";
}

}  // namespace switchbot_keypad_bridge
}  // namespace esphome
