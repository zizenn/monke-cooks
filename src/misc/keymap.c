#include "game/keymap.h"

// Define the default controls. You can map multiple keys to the same action!
Keymap keymaps[] = {
    { KEY_W, ACTION_UP },
    { KEY_UP, ACTION_UP },

    { KEY_S, ACTION_DOWN },
    { KEY_DOWN, ACTION_DOWN },

    { KEY_A, ACTION_LEFT },
    { KEY_LEFT, ACTION_LEFT },

    { KEY_D, ACTION_RIGHT },
    { KEY_RIGHT, ACTION_RIGHT },

    { KEY_SPACE, ACTION_INTERACT },
    { KEY_E, ACTION_INTERACT }
};

#define TOTAL_MAPPINGS (sizeof(keymaps) / sizeof(keymaps[0]))

bool IsActionPressed(KEYBOARD_ACTION action) {
    for (int i = 0; i < TOTAL_MAPPINGS; i++) {
        if (keymaps[i].action == action) {
            if (IsKeyPressed(keymaps[i].key)) {
                return true;
            }
        }
    }
    return false;
}

bool IsActionDown(KEYBOARD_ACTION action) {
    for (int i = 0; i < TOTAL_MAPPINGS; i++) {
        if (keymaps[i].action == action) {
            if (IsKeyDown(keymaps[i].key)) {
                return true;
            }
        }
    }
    return false;
}
