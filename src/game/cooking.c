#include "game/cooking.h"
#include "external/raylib.h"
#include "external/raygui.h"
#include "game/screens.h"
#include "game/display_screen.h"
#include "game/globals.h"
#include "minigames/minigame.h"
#include "game/game.h"
#include "game/items.h"
#include "string.h"

//minigames
#include "minigames/timingbar.h"
#include "minigames/targetgame.h"
#include "minigames/basketcatch.h"

static char panelTitle[9] = "";
int minigameSelection;
const char* debugText;

static enum {
  RUNNING,
  WIN,
  LOSE
} MinigameStatus;

static whereIsItemFrom newItemFrom;
static bool cookResultApplied = false;
ItemType newHolding;

void InitCook() {
  MinigameStatus = false;
  cookResultApplied = false;

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

  minigameSelection = GetRandomValue(0, 2);

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
  debugText = categories[categoryId].variants[nextVariantId].name;

  switch (minigameSelection) {
    case 0:
      InitBarMinigame();
      break;
    case 1:
      InitTargetMinigame();
      break;
    case 2:
      InitBasketMinigame();
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
      UpdateBarMinigame();
      if (timingBarResult == TIMING_BAR_WIN) {
        MinigameStatus = WIN;
      } else if (timingBarResult == TIMING_BAR_LOSE) {
        MinigameStatus = LOSE;
      } else {
        MinigameStatus = RUNNING;
      }
      break;
    case 1:
      UpdateTargetMinigame();
      if (targetGameResult == TARGET_GAME_WIN) {
        MinigameStatus = WIN;
      } else if (targetGameResult == TARGET_GAME_LOSE) {
        MinigameStatus = LOSE;
      } else {
        MinigameStatus = RUNNING;
      }
      break;
    case 2:
      UpdateBasketMinigame();
      if (basketCatchResult == BASKET_CATCH_WIN) {
        MinigameStatus = WIN;
      } else if (basketCatchResult == BASKET_CATCH_LOSE) {
        MinigameStatus = LOSE;
      } else {
        MinigameStatus = RUNNING;
      }
      break;
  }

  if (MinigameStatus == WIN && !cookResultApplied) {
    // Textures are preloaded and persistent - no need to load/unload
    holding = newHolding;
    itemFrom = newItemFrom;
    TraceLog(LOG_INFO, "item name: %s", debugText);
    cookResultApplied = true;
    UnloadCook();
    currentScreen = GAME;
  }
  if (MinigameStatus == LOSE) {
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
    case 1:
      DrawTargetMinigame();
      break;
    case 2:
      DrawBasketMinigame();
      break;
  }
}

void UnloadCook() {
  switch (minigameSelection) {
    case 0:
      UnloadBarMinigame();
      break;
    case 1:
      UnloadTargetMinigame();
      break;
    case 2:
      UnloadBasketMinigame();
      break;
  }
}
