#include "game/display_screen.h"
#include "external/raylib.h"
#include "external/raygui.h"
#include "game/screens.h"
#include "game/globals.h"

static Music menu_music;
static Sound clickSound;
static Texture2D monke;

static Rectangle loadBtn        = { 15, 650,  200, 50 };
static Rectangle advancementBtn = { 230, 650, 200, 50 };
static Rectangle restartBtn     = { 445, 650, 200, 50 };
static Rectangle quitBtn        = { 660, 650, 200, 50 };

void InitCampaignMenu() {
  menu_music = LoadMusicStream("assets/brackeys/music/time_for_adventure.mp3");
  clickSound = LoadSound("assets/brackeys/sounds/tap.wav");
  monke = LoadTexture("assets/monke_front.png");
  PlayMusicStream(menu_music);
}

void UpdateCampaignMenu() {
  UpdateMusicStream(menu_music);
}

void DrawCampaignMenu(void) {
    ClearBackground(RAYWHITE);
    DrawText("Campaign Mode", 15, 15, 35, BLACK);

    if (GuiButton(loadBtn, "Start/Continue")) {
        PlaySound(clickSound);
        currentScreen = GAME;
    }

    if (GuiButton(advancementBtn, "Advancements")) {
        PlaySound(clickSound);
        // current = ADVANCEMENTS_MENU;
    }

    if (GuiButton(restartBtn, "[DANGER] Reset")) {
        PlaySound(clickSound);
    }

    if (GuiButton(quitBtn, "Back")) {
        PlaySound(clickSound);
        currentScreen = MAIN_MENU;
    }
}

void UnloadCampaignMenu() {
    UnloadMusicStream(menu_music);
    UnloadTexture(monke);
    UnloadSound(clickSound);
}
