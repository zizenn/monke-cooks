#include "game/display_screen.h"
#include "external/raylib.h"
#include "external/raygui.h"
#include "game/screens.h"
#include "game/globals.h"
#include "game/buttons.h"

static Music menu_music;
static Sound clickSound;

static Rectangle rectangles[] = {{ 15, 650,  200, 50 }, //Enter
                                 { 230, 650, 200, 50 }, //Advanced
                                 { 445, 650, 200, 50 }, //Reset
                                 { 660, 650, 200, 50 }}; //Back

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
