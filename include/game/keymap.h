#ifndef KEYMAP_H
#define KEYMAP_H

#include "external/raylib.h"

typedef enum {
    ACTION_UP,
    ACTION_DOWN,
    ACTION_LEFT,
    ACTION_RIGHT,
    ACTION_INTERACT,
    ACTION_COUNT
} KEYBOARD_ACTION;

typedef struct {
    KeyboardKey key;
    KEYBOARD_ACTION action;
} Keymap;

extern Keymap keymaps[];

bool IsActionPressed(KEYBOARD_ACTION action);  // Checks if key was pressed *this frame* (good for menus/interactions)
bool IsActionDown(KEYBOARD_ACTION action);     // Checks if key is being held *continuous down* (good for walking)

#endif
