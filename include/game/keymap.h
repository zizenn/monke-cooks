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

extern Keymap* keymaps;
extern int totalKeymaps;

void LoadKeymap(const char* filename);
void UnloadKeymap(void);
bool IsActionPressed(KEYBOARD_ACTION action);
bool IsActionDown(KEYBOARD_ACTION action);

#endif
