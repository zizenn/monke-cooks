#ifndef GLOBALS_H
#define GLOBALS_H

#include "core/enums.h"
#include "core/types.h"
#include "external/raylib.h"

// Game state
extern GameScreen currentScreen;
extern bool shouldQuit;
extern RenderTexture2D canvas;
extern int targetFPS;

// Cooking state
extern COOK_TYPE currentCookType;
extern PREP_TYPE currentPrepType;
extern PREP_TYPE prepFrom;

// Player holding
extern Holding holding;

// UI
extern Rectangle notifPanelBounds;

// Notifications
void SummonNotif(const char* text, NOTIF_TYPE notifType);
void UpdateNotifications();
void DrawNotifications();

#endif
