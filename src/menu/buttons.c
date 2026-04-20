#include "game/buttons.h"
#include "external/raylib.h"

ButtonStyle ButtonStyles[BUTTON_LEN_COUNT] = {0};

bool DrawTexturedButton(Rectangle bounds, const char *label, ButtonStyleID styleID) {
    ButtonStyle *s = &ButtonStyles[styleID];


    if (s->hasTexture) {
        if (s->nineSlice) {
            DrawNineSlice(s->texture, bounds, s->borderTop, s->borderBottom, s->borderLeft, s->borderRight, s->tint);
        } else {
            DrawTexturePro(
                s->texture,
                (Rectangle){0, 0, s->texture.width, s->texture.height},
                bounds,
                (Vector2){0}, 
                0.0f,
                s->tint
            );
        }


        bool hovered = CheckCollisionPointRec(GetMousePosition(), bounds);

        int prevTextNormal  = GuiGetStyle(BUTTON, TEXT_COLOR_NORMAL);
        int prevTextFocused = GuiGetStyle(BUTTON, TEXT_COLOR_FOCUSED);

        int prevBaseNormal    = GuiGetStyle(BUTTON, BASE_COLOR_NORMAL);
        int prevBaseFocused   = GuiGetStyle(BUTTON, BASE_COLOR_FOCUSED);
        int prevBasePressed   = GuiGetStyle(BUTTON, BASE_COLOR_PRESSED);
        int prevBorderNormal  = GuiGetStyle(BUTTON, BORDER_COLOR_NORMAL);
        int prevBorderFocused  = GuiGetStyle(BUTTON, BORDER_COLOR_FOCUSED);
        int prevBorderPressed  = GuiGetStyle(BUTTON, BORDER_COLOR_PRESSED);

        GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL,  ColorToInt(s->textColor));
        GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(s->textFocused));

        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL,    0x00000000);
        GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED,   0x00000000);
        GuiSetStyle(BUTTON, BASE_COLOR_PRESSED,   0x00000000);
        GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL,  0x00000000);
        GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED,  0x00000000);
        GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED,  0x00000000);

        bool clicked = GuiButton(bounds, label); //detect before rendering texture over

        GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL,  prevTextNormal);
        GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, prevTextFocused);
        
        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL,    prevBaseNormal);
        GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED,   prevBaseFocused);
        GuiSetStyle(BUTTON, BASE_COLOR_PRESSED,   prevBasePressed);
        GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL,  prevBorderNormal);
        GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL,  prevBorderFocused);
        GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL,  prevBorderPressed);


        return clicked; //detect button
    }

    return GuiButton(bounds, label); //if no texture provided, use default detection.

}
void DrawNineSlice(Texture2D texture, Rectangle dest, int bTop, int bBottom, int bLeft, int bRight, Color tint) {
    int sw = texture.width;
    int sh = texture.height;

    //source
    int centerSourceW = sw - bLeft - bRight;
    int centerSourceH = sh - bTop - bBottom;

    //destination
    int centerDestW = dest.width - bLeft - bRight;
    int centerDestH = dest.height - bTop - bBottom;

    //corners
    //tl
    DrawTexturePro(texture, (Rectangle){0,           0,            bLeft,  bTop   }, (Rectangle){dest.x,                       dest.y,                         bLeft,  bTop   }, (Vector2){0}, 0, tint);
    //tr
    DrawTexturePro(texture, (Rectangle){sw - bRight, 0,            bRight, bTop   }, (Rectangle){dest.x + dest.width - bRight, dest.y,                         bRight, bTop   }, (Vector2){0}, 0, tint);
    //bl
    DrawTexturePro(texture, (Rectangle){0,           sh - bBottom, bLeft,  bBottom}, (Rectangle){dest.x,                       dest.y + dest.height - bBottom, bLeft,  bBottom}, (Vector2){0}, 0, tint);
    //br
    DrawTexturePro(texture, (Rectangle){sw - bRight, sh - bBottom, bRight, bTop}, (Rectangle){dest.x + dest.width - bRight, dest.y + dest.height - bBottom, bRight, bBottom}, (Vector2){0}, 0, tint);

    //edges
    //t
    DrawTexturePro(texture, (Rectangle){bLeft,       0,            centerSourceW, bTop         }, (Rectangle){dest.x + bLeft,               dest.y,                         centerDestW, bTop       }, (Vector2){0}, 0, tint);
    //b
    DrawTexturePro(texture, (Rectangle){bLeft,       sh - bBottom, centerSourceW, bBottom         }, (Rectangle){dest.x + bLeft,               dest.y + dest.height - bBottom, centerDestW, bBottom    }, (Vector2){0}, 0, tint);
    //l
    DrawTexturePro(texture, (Rectangle){0,           bTop,         bLeft,         centerSourceH}, (Rectangle){dest.x,                       dest.y + bTop,                  bLeft,       centerDestH}, (Vector2){0}, 0, tint);
    //r
    DrawTexturePro(texture, (Rectangle){sw - bRight, bTop,         bRight,        centerSourceH}, (Rectangle){dest.x + dest.width - bRight, dest.y + bTop,                  bRight,      centerDestH}, (Vector2){0}, 0, tint);

    //center
    DrawTexturePro(texture, (Rectangle){bLeft, bTop, centerSourceW, centerSourceH}, (Rectangle){dest.x + bLeft, dest.y + bTop, centerDestW, centerDestH}, (Vector2){0}, 0, tint);
}
