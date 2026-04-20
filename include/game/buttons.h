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
    Color       textColor;
    Color       textFocused;
    //nine-slice border sizes
    int         borderTop;
    int         borderBottom;
    int         borderLeft;
    int         borderRight;
    bool        nineSlice;
} ButtonStyle;

void DrawNineSlice(Texture2D texture, Rectangle dest, int bTop, int bBottom, int bLeft, int bRight, Color tint);

extern ButtonStyle ButtonStyles[BUTTON_LEN_COUNT];

bool DrawTexturedButton(Rectangle bounds, const char *label, ButtonStyleID styleID);

#endif