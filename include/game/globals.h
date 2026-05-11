#ifndef GLOBALS_H
#define GLOBALS_H

#include "core/enums.h"
#include "core/types.h"
#include "external/raylib.h"

// game state
extern SCREENS currentScreen;
extern bool shouldQuit;
extern RenderTexture2D canvas;
extern int targetFPS;

// cooking state
extern COOK_TYPE currentCookType;
extern PREP_TYPE currentPrepType;
extern PREP_TYPE prepFrom;

// inventory state
extern ITEM_ORIGIN currentInventoryType;

// player holding
extern Holding holding;

// UI
extern Rectangle notifPanelBounds;

// notifications
void SummonNotif(const char* text, NOTIF_TYPE notifType);
void UpdateNotifications();
void DrawNotifications();

#endif