#ifndef SCREEN_MENU_H
#define SCREEN_MENU_H

#include "core/enums.h"
#include "stdbool.h"

// --- SCENE MANAGEMENT ---
void PushScene(SCREENS scene);
void PopScene(void);
bool IsOverlayActive(void);

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

// recipe book
void InitRecipeBook(void);
void UpdateRecipeBook(void);
void DrawRecipeBook(void);
void UnloadRecipeBook(void);

// --- GAME ---

// base game
void InitGame(void);
void UpdateGame(void);
void DrawGame(void);
void UnloadGame(void);

// inventory
void InitInventory(void);
void UpdateInventory(void);
void DrawInventory(void);
void UnloadInventory(void);

// cooking surfaces (stove, oven, etc.)
void InitCook(void);
void UpdateCook(void);
void DrawCook(void);
void UnloadCook(void);

// prep surfaces
void PrepFood(int currentTile);

// minigame
void InitBarMinigame(void);
void UpdateBarMinigame(void);
void DrawBarMinigame(void);
void UnloadBarMinigame(void);

void InitTargetMinigame(void);
void UpdateTargetMinigame(void);
void DrawTargetMinigame(void);
void UnloadTargetMinigame(void);

void InitBasketMinigame(void);
void UpdateBasketMinigame(void);
void DrawBasketMinigame(void);
void UnloadBasketMinigame(void);

#endif
