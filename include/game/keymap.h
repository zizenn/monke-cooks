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

typedef struct {
  const char* keyString;
  int keyValue;
} StringMapping;

// variables
extern Keymap* keymaps;
extern int totalKeymaps;

// LUT predefentions
extern const StringMapping KeyLUT[];
extern const StringMapping ActionLUT[];
extern const int KeyLUTSize;
extern const int ActionLUTSize;

// function prototypes
void LoadKeymap(const char* filename);
void UnloadKeymap(void);
bool IsActionPressed(KEYBOARD_ACTION action);
bool IsActionDown(KEYBOARD_ACTION action);

#endif
