#include "game/cooking.h"
#include "external/raylib.h"
#include "external/raygui.h"
#include "game/screens.h"
#include "game/display_screen.h"
#include "game/globals.h"
#include "game/game.h"
#include "game/items.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

COOK_TYPE currentCookType;
static char panelTitle[9] = "";

void InitCook() {
  switch (currentCookType) {
    case PAN:
      strcpy(panelTitle, "stove");
      break;
    case DEEP_FRY:
      strcpy(panelTitle, "deep fry");
      break;
    case OVEN:
      strcpy(panelTitle, "oven");
      break;
    case GRILL:
      strcpy(panelTitle, "grill");
      break;
  }
}

void UpdateCook() {
  if (IsKeyPressed(KEY_ESCAPE)) {
    UnloadCook();
    currentScreen = GAME;
  } else if (IsKeyPressed(KEY_ENTER)) {
    UnloadCook();
    currentScreen = GAME;
  }
}

void DrawCook() {
  int screenWidth = GetScreenWidth();
  int screenHeight = GetScreenHeight();

  Rectangle totalArea = {(screenWidth/2)-346.5, (screenHeight/2)-178.5, 693, 357};
  Rectangle panelBounds = {totalArea.x, totalArea.y, totalArea.width, totalArea.height-16};

  GuiPanel(panelBounds, panelTitle);
}

void UnloadCook() {

}
