#pragma once

// SwitchBot physical lock battery extraction from BLE advertisements.

#include <cstddef>
#include <cstdint>

#include "physical_lock.h"

namespace esphome {
namespace switchbot_keypad_bridge {

// `manufacturer_payload` is the SwitchBot manufacturer payload after the
// 2-byte company id (0x0969). Returns -1 when the advert does not carry a
// battery field for the supplied model.
int parse_lock_battery(PhysicalLockModel model,
                       const uint8_t *service_data, size_t service_len,
                       const uint8_t *manufacturer_payload,
                       size_t manufacturer_len);

// Lock state and door sensor from the same manufacturer payload. Bit layout
// mirrors pySwitchbot's adv_parsers/lock.py. -1 = not carried by this model.
struct LockAdvertState {
  int status{-1};     // pySwitchbot LockStatus: 0 locked .. 7 half locked
  int door_open{-1};  // 1 open, 0 closed
};
LockAdvertState parse_lock_state(PhysicalLockModel model,
                                 const uint8_t *manufacturer_payload,
                                 size_t manufacturer_len);
const char *lock_status_str(int status);

}  // namespace switchbot_keypad_bridge
}  // namespace esphome
