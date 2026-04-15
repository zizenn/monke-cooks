#include "game/display_screen.h"
#include "external/raylib.h"
#include "external/raygui.h"
#include "game/screens.h"
#include "game/globals.h"

static Music menu_music;
static Sound clickSound;
static Texture2D monke;

static Rectangle campaignBtn    = { 15, 70,  200, 50 };
static Rectangle multiplayerBtn = { 15, 150, 200, 50 };
static Rectangle settingsBtn    = { 15, 230, 200, 50 };
static Rectangle quitBtn        = { 15, 310, 200, 50 };

void InitMainMenu(void);
void UpdateMainMenu(void);
void DrawMainMenu(void);
void UnloadMainMenu(void);

void UpdateMainMenu() {
    UpdateMusicStream(menu_music);
}

void DrawMainMenu(void) {
    ClearBackground(RAYWHITE);
    DrawText("monke cooks", 15, 15, 35, BLACK);

    if (GuiButton(campaignBtn, "Campaign")) {
        PlaySound(clickSound);
        current = CAMPAIGN_MENU;
    }

    if (GuiButton(multiplayerBtn, "Multiplayer")) {
        PlaySound(clickSound);
        // current = MULTIPLAYER_MENU;
    }

    if (GuiButton(settingsBtn, "Settings")) {
        PlaySound(clickSound);
        // current = SETTINGS_MENU;
    }

    if (GuiButton(quitBtn, "Quit")) {
        PlaySound(clickSound);
        shouldQuit = true;
    }
}

void InitMainMenu() {
  menu_music = LoadMusicStream("assets/brackeys/music/time_for_adventure.mp3");
  clickSound = LoadSound("assets/brackeys/sounds/tap.wav");
  monke = LoadTexture("assets/monke_front.png");
  PlayMusicStream(menu_music);
}

void UnloadMainMenu() {
    UnloadMusicStream(menu_music);
    UnloadTexture(monke);
    UnloadSound(clickSound);
}
