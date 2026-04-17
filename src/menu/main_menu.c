#include "game/display_screen.h"
#include "external/raylib.h"
#include "external/raygui.h"
#include "game/screens.h"
#include "game/globals.h"

// function prototypes
static int menuNavigation(Rectangle *rects, int count, int *selected);

static Music menu_music;
static Sound clickSound;

static bool isMenuOpen = false;
static bool spaceWasPressed = false;
static MENU_TYPE currentMenu = NONE;

static Rectangle rectangles[] = {
  { 15, 70,  200, 50 }, // campaign
  { 15, 150, 200, 50 }, // multiplayer
  { 15, 230, 200, 50 }, // settings
  { 15, 310, 200, 50 }  // quit
};

void UpdateMainMenu() {
    UpdateMusicStream(menu_music);
}

void DrawMainMenu(void) {
    ClearBackground(RAYWHITE);
    DrawText("monke cooks", 15, 15, 35, BLACK);

    if (GuiButton(rectangles[0], "Campaign")) {
        PlaySound(clickSound);
        currentScreen = CAMPAIGN_MENU;
    }

    if (GuiButton(rectangles[1], "Multiplayer")) {
        PlaySound(clickSound);
        // current = MULTIPLAYER_MENU;
    }

    if (GuiButton(rectangles[2], "Settings")) {
        PlaySound(clickSound);
        // current = SETTINGS_MENU;
    }

    if (GuiButton(rectangles[3], "Quit")) {
        PlaySound(clickSound);
        shouldQuit = true;
    }
}

void InitMainMenu() {
  menu_music = LoadMusicStream("assets/brackeys/music/time_for_adventure.mp3");
  clickSound = LoadSound("assets/brackeys/sounds/tap.wav");
  PlayMusicStream(menu_music);
}

void UnloadMainMenu() {
  UnloadMusicStream(menu_music);
  UnloadSound(clickSound);
}

static int menuNavigation(Rectangle *rects, int count, int *selected) {
  if (IsKeyPressed(KEY_D)) (*selected)++;
  if (IsKeyPressed(KEY_A)) (*selected)--;
  if (*selected < 0) *selected = 0;
  if (*selected >= count) *selected = count - 1;

  // draw a rectangle behind to show selection (1 pixel larger on all sides)
  Rectangle selectionRect = {
    rects[*selected].x - 1,
    rects[*selected].y - 1,
    rects[*selected].width + 2,
    rects[*selected].height + 2
  };
  DrawRectangleRec(selectionRect, YELLOW);

  if (IsKeyPressed(KEY_C)) {
    isMenuOpen = false;
    currentMenu = NONE;
    return -1;
  }

  if (IsKeyPressed(KEY_SPACE) && !spaceWasPressed) return *selected + 1;
  return -1;
}
