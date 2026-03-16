#ifndef GLOBALS_H
#define GLOBALS_H

#include "raylib.h"
#include "screen_manager.h"

extern Vector2 mousePos;
extern GameScreen current;
extern bool shouldQuit;

//Sakif PLEASE why'd you define "hovered" and "clicked" differently within main.c and m_menu.c???
// eaton, its cuz im autistic and dumb idk what i even did
static inline bool hovered(Rectangle rec) {
  return CheckCollisionPointRec(mousePos, rec); 
}

static inline bool clicked(int mouseBtn, Rectangle rec) {
  return IsMouseButtonPressed(mouseBtn) && hovered(rec);
}

#endif