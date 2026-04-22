#include "game/display_screen.h"
#include "external/raylib.h"
#include "external/raygui.h"
#include "game/screens.h"
#include "game/globals.h"
#include "game/buttons.h"

static Music menu_music;
static Sound clickSound;

static Rectangle rectangles[] = {{ 12, 520, 160, 40 }, //Enter
                                 { 184, 520, 160, 40 }, //Advanced
                                 { 356, 520, 160, 40 }, //Reset
                                 { 528, 520, 160, 40 }}; //Back

void InitCampaignMenu() {
  menu_music = LoadMusicStream("assets/brackeys/music/time_for_adventure.mp3");
  clickSound = LoadSound("assets/brackeys/sounds/tap.wav");
  PlayMusicStream(menu_music);
}

void UpdateCampaignMenu() {
  UpdateMusicStream(menu_music);
}

void DrawCampaignMenu(void) {
    ClearBackground(RAYWHITE);
    DrawText("Campaign Mode", 15, 15, 35, BLACK);

    if (DrawTexturedButton(rectangles[0], "Play Campaign", BUTTON_STYLE_PRIMARY)) {
        PlaySound(clickSound);
        currentScreen = GAME;
    }

    if (DrawTexturedButton(rectangles[1], "MinigameTEST", BUTTON_STYLE_PRIMARY)) {
        PlaySound(clickSound);
        currentScreen = BARMINIGAME_SCREEN;
    }

    if (DrawTexturedButton(rectangles[2], "RESET", BUTTON_STYLE_DANGER)) {
        PlaySound(clickSound);
    }

    if (DrawTexturedButton(rectangles[3], "Back", BUTTON_STYLE_PRIMARY)) {
        PlaySound(clickSound);
        currentScreen = MAIN_MENU;
    }
}

void UnloadCampaignMenu() {
    UnloadMusicStream(menu_music);
    UnloadSound(clickSound);
}
