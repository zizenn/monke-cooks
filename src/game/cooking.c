#include "game/cooking.h"
#include "external/raylib.h"
#include "external/raygui.h"
#include "game/screens.h"
#include "game/display_screen.h"
#include "game/globals.h"
#include "minigames/minigame.h"
#include "game/game.h"
#include "game/items.h"
#include "game/texture_cache.h"
#include "stdint.h"
#include "string.h"

static char panelTitle[9] = "";
int minigameSelection;
static bool minigameWin = false;
static whereIsItemFrom newItemFrom;
static bool cookResultApplied = false;
static Texture2D cookedItemTex;
ItemType newHolding;

void InitCook() {
  minigameWin = false;
  cookResultApplied = false;
  cookedItemTex = (Texture2D){0};

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

  FoodCategory *categories = NULL;
  if (itemFrom == FROM_FRIDGE) {
    categories = allFoods;
  } else if (itemFrom == FROM_PANTRY) {
    categories = allPantry;
  } else {
    return;
  }

  int categoryId = holding.categoryId;
  int nextVariantId = holding.variantId + 1;
  if (categoryId < 0 || nextVariantId >= categories[categoryId].variantCount) {
    return;
  }

  Foods nextVariant = categories[categoryId].variants[nextVariantId];
  newHolding = (ItemType){categoryId, nextVariantId, nextVariant.cook_type};
  cookedItemTex = AcquireCachedTexture(nextVariant.filePath);

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
    return;
  } else if (IsKeyPressed(KEY_ENTER)) {
    UnloadCook();
    currentScreen = GAME;
    return;
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

  if (minigameWin && !cookResultApplied && cookedItemTex.id != 0) {
    for (int i = 0; i < 4; i++) {
      if (playerTexture[i].id != cookedItemTex.id) {
        ReleaseTexture(playerTexture[i]);
      }
    }
    FillTextureArray(playerTexture, 4, cookedItemTex);
    holding = newHolding;
    cookResultApplied = true;
    UnloadCook();
    currentScreen = GAME;
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
  if (cookedItemTex.id != 0 && !cookResultApplied) {
    ReleaseTexture(cookedItemTex);
  }
  cookedItemTex = (Texture2D){0};

  switch (minigameSelection) {
    case 0:
      UnloadBarMinigame();
      break;
  }
}
