#ifndef GLOBALS_H
#define GLOBALS_H

#include "external/raylib.h"
#include "game/screens.h"

typedef enum {
  NONE,
  FRIDGE_MENU,
  PANTRY_MENU,
  STOVE_MENU,
  OVEN_MENU,
  DEEP_FRY_MENU,
  GRILL_MENU
} MENU_TYPE;

typedef enum {
  INFO,
  WARNING,
  ERROR,
  SUCCESS
} NOTIF_TYPE;

extern GameScreen currentScreen;
extern bool shouldQuit;
extern RenderTexture2D canvas;
extern int targetFPS;

extern Rectangle notifPanelBounds;

void UpdateUILayoutRects();
void summonNotif(const char* text, NOTIF_TYPE notifType);
void UpdateNotifications();
void DrawNotifications();

#endif
