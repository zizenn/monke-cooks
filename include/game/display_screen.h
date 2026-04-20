#ifndef SCREEN_MENU_H
#define SCREEN_MENU_H

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

// stove
void InitCook();
void UpdateCook();
void DrawCook();
void UnloadCook();

// minigame
void InitBarMinigame(void);
void UpdateBarMinigame(void);
void DrawBarMinigame(void);
void UnloadBarMinigame(void);

#endif
