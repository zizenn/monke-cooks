#include "game/keymap.h"
#include "core/map.h"

// LUT meants lookup table

// KEYMAP LUTs
const StringMapping KeyLUT[] = {
  { "KEY_W", KEY_W }, { "KEY_S", KEY_S }, { "KEY_A", KEY_A }, { "KEY_D", KEY_D },
  { "KEY_UP", KEY_UP }, { "KEY_DOWN", KEY_DOWN }, { "KEY_LEFT", KEY_LEFT }, { "KEY_RIGHT", KEY_RIGHT },
  { "KEY_SPACE", KEY_SPACE }, { "KEY_E", KEY_E }
};

const StringMapping ActionLUT[] = {
  { "ACTION_UP", ACTION_UP },
  { "ACTION_DOWN", ACTION_DOWN },
  { "ACTION_LEFT", ACTION_LEFT },
  { "ACTION_RIGHT", ACTION_RIGHT },
  { "ACTION_INTERACT", ACTION_INTERACT }
};

const int KeyLUTSize = sizeof(KeyLUT) / sizeof(KeyLUT[0]);
const int ActionLUTSize = sizeof(ActionLUT) / sizeof(ActionLUT[0]);

// TILE ACTIONS LUT
