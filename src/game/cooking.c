#include "game/cooking.h"
#include "external/raylib.h"
#include "external/raygui.h"
#include "game/screens.h"
#include "game/display_screen.h"
#include "game/globals.h"
#include "minigames/minigame.h"
#include "game/game.h"
#include "game/items.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

COOK_TYPE currentCookType;
static char panelTitle[9] = "";
int minigameSelection;

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

  minigameSelection = GetRandomValue(0, 0);

  switch (minigameSelection) {
    case 0:
      InitBarMinigame();
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

  switch (minigameSelection) {
    case 0:
      UpdateBarMinigame();
      break;
  }
}

void DrawCook() {
  GuiPanel(panelBounds, panelTitle);

  switch (minigameSelection) {
    case 0:
      DrawBarMinigame();
      break;
  }

}

void UnloadCook() {
  switch (minigameSelection) {
    case 0:
      UnloadBarMinigame();
      break;
  }
}
