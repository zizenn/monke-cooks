#ifndef GLOBALS_H
#define GLOBALS_H

#include "external/raylib.h"
#include "game/screens.h"

extern GameScreen currentScreen;
extern bool shouldQuit;
extern RenderTexture2D canvas;

typedef enum {
  NONE,
  FRIDGE_MENU,
  STOVE_MENU
} MENU_TYPE;

#endif
