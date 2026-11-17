#!/bin/sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
BIN=$(mktemp "${TMPDIR:-/tmp}/switchbot-lock-advert-test.XXXXXX")
trap 'rm -f "$BIN"' EXIT HUP INT TERM
CXX=${CXX:-c++}

"$CXX" -std=c++17 -Wall -Wextra -Werror -I"$ROOT" -I"$ROOT/tests/stubs" \
  "$ROOT/components/switchbot_keypad_bridge/lock_advert.cpp" \
  "$ROOT/tests/lock_advert_test.cpp" -o "$BIN"
"$BIN"
