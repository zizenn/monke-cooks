#ifndef SCREEN_MENU_H
#define SCREEN_MENU_H

#include "external/raylib.h"

// --- MENUS ---

// main menu
void InitMainMenu(void);
void UpdateMainMenu(void);
void DrawMainMenu(void);
void UnloadMainMenu(void);

// campaign menu
void InitCampaignMenu(void);
void UpdateCampaignMenu(void);
void DrawCampaignMenu(void);
void UnloadCampaignMenu(void);

// --- GAME ---

// base game
void InitGame(void);
void UpdateGame(void);
void DrawGame(void);
void UnloadGame(void);

// fridge
void InitFridge(void);
void UpdateFridge(void);
void DrawFridge(void);
void UnloadFridge(void);

// pantry
void InitPantry(void);
void UpdatePantry(void);
void DrawPantry(void);
void UnloadPantry(void);

// cooking surfaces (stove, oven, etc.)
void InitCook();
void UpdateCook();
void DrawCook();
void UnloadCook();

// prep surfaces
// void InitCook();
// void UpdateCook();
// void DrawCook();
// void UnloadCook();

// followbar minigame
void InitBarMinigame(void);
bool UpdateBarMinigame(void);
void DrawBarMinigame(void);
void UnloadBarMinigame(void);

// target minigame
void InitTargetMinigame(void);
void UpdateTargetMinigame(void);
void DrawTargetMinigame(void);
void UnloadTargetMinigame(void);

#endif
