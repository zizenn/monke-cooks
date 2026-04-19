#ifndef BUTTONS_H
#define BUTTONS_H

#include "external/raylib.h"
#include "external/raygui.h"

// Button style IDs
typedef enum {
    BUTTON_STYLE_DEFAULT = 0,
    BUTTON_STYLE_PRIMARY,
    BUTTON_STYLE_DANGER,
    BUTTON_LEN_COUNT
} ButtonStyleID;

typedef struct {
    Texture2D   texture;
    bool        hasTexture;
    Color       tint;
} ButtonStyle;

extern ButtonStyle ButtonStyles[BUTTON_LEN_COUNT];

bool DrawTexturedButton(Rectangle bounds, const char *label, ButtonStyleID styleID);

#endif