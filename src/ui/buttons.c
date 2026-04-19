#include "game/buttons.h"

ButtonStyle ButtonStyles[BUTTON_LEN_COUNT] = {0};

bool DrawTexturedButton(Rectangle bounds, const char *label, ButtonStyleID styleID) {
    ButtonStyle *s = &ButtonStyles[styleID];

    if (s->hasTexture) {
        DrawTexturePro(
            s->texture,
            (Rectangle){0, 0, s->texture.width, s->texture.height},
            bounds,
            (Vector2){0, 0}, 
            0.0f,
            s->tint
        );
    }

    return GuiButton(bounds, label);
}