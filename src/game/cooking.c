#include "game/cooking.h"
#include "external/raylib.h"
#include "external/raygui.h"
#include "game/screens.h"
#include "game/display_screen.h"
#include "game/globals.h"
#include "minigames/minigame.h"
#include "game/game.h"
#include "game/items.h"
#include "stdint.h"
#include "string.h"

static char panelTitle[9] = "";
int minigameSelection;
static bool minigameWin = false;
static whereIsItemFrom newItemFrom;
Texture2D fridgeItemTex;
Texture2D pantryItemTex;
itemType newHolding;

void InitCook() {
  minigameWin = false;

  switch (currentCookType) {
    case PAN:
      strcpy(panelTitle, "stove");
      newItemFrom = FROM_STOVE;
      break;
    case DEEP_FRY:
      strcpy(panelTitle, "deep fry");
      newItemFrom = FROM_DEEP_FRY;
      break;
    case OVEN:
      strcpy(panelTitle, "oven");
      newItemFrom = FROM_OVEN;
      break;
    case GRILL:
      strcpy(panelTitle, "grill");
      newItemFrom = FROM_GRILL;
      break;
    case 0:
      break;
  }

  minigameSelection = GetRandomValue(0, 0);

  newHolding = (itemType){holding.categoryId, holding.variantId++, 0};
  fridgeItemTex = LoadTexture(allFoods[newHolding.categoryId].variants[newHolding.variantId].filePath);
  pantryItemTex = LoadTexture(allPantry[newHolding.categoryId].variants[newHolding.variantId].filePath);

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
    if (itemFrom == FROM_FRIDGE) {
      for (int i = 0; i < 4; i++) {
        playerTexture[i] = fridgeItemTex;
      }
    } else if (itemFrom == FROM_PANTRY) {
      for (int i = 0; i < 4; i++) {
        playerTexture[i] = pantryItemTex;
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
