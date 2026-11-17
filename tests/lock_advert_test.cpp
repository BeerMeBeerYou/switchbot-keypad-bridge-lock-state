#include <cassert>
#include <cstdint>
#include <cstring>

#include "components/switchbot_keypad_bridge/lock_advert.h"

using esphome::switchbot_keypad_bridge::LockAdvertState;
using esphome::switchbot_keypad_bridge::PhysicalLockModel;
using esphome::switchbot_keypad_bridge::lock_status_str;
using esphome::switchbot_keypad_bridge::parse_lock_state;

int main() {
  // Lock Pro: mfr[7] bits 6..3 = status, mfr[8] bit 4 = door open.
  uint8_t pro[12] = {};
  pro[7] = 1 << 3;  // unlocked
  pro[8] = 0x10;    // door open
  LockAdvertState s = parse_lock_state(PhysicalLockModel::LOCK_PRO, pro, sizeof(pro));
  assert(s.status == 1 && s.door_open == 1);
  assert(std::strcmp(lock_status_str(s.status), "Unlocked") == 0);

  pro[7] = 0x80;  // calibration bit only -> locked
  pro[8] = 0x08;  // auto-lock paused, door closed
  s = parse_lock_state(PhysicalLockModel::LOCK_ULTRA, pro, sizeof(pro));
  assert(s.status == 0 && s.door_open == 0);

  // Too short -> nothing.
  s = parse_lock_state(PhysicalLockModel::LOCK_PRO, pro, 11);
  assert(s.status == -1 && s.door_open == -1);

  // Original Lock: mfr[7] bits 6..4 = status, bit 2 = door open.
  uint8_t lock[9] = {};
  lock[7] = (3 << 4) | 0x04;
  s = parse_lock_state(PhysicalLockModel::LOCK, lock, sizeof(lock));
  assert(s.status == 3 && s.door_open == 1);

  // Lock Lite has no door sensor.
  s = parse_lock_state(PhysicalLockModel::LOCK_LITE, lock, sizeof(lock));
  assert(s.status == 3 && s.door_open == -1);

  assert(std::strcmp(lock_status_str(9), "Unknown") == 0);
  return 0;
}
