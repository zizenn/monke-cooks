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
#include "stdint.h"
#include "string.h"

static char panelTitle[9] = "";
int minigameSelection;
static bool minigameWin = false;

void InitCook() {
  minigameWin = false;

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
    case 0:
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
      if (UpdateBarMinigame()) {
        minigameWin = true;
      } else {
        minigameWin = false;
      }
      break;
  }

  if (minigameWin) {
    itemType newHolding = (itemType){holding.categoryId, holding.variantId++, 0};

    if (itemFrom == FROM_FRIDGE) {
      for (int i = 0; i < 4; i++) {
        playerTexture[i] = LoadTexture(allFoods[newHolding.categoryId].variants[newHolding.variantId].filePath);
      }
    } else if (itemFrom == FROM_PANTRY) {
      for (int i = 0; i < 4; i++) {
        playerTexture[i] = LoadTexture(allPantry[newHolding.categoryId].variants[newHolding.variantId].filePath);
      }
    }

    holding = newHolding;
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
