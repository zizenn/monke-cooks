#include "external/raylib.h"
#include "external/raygui.h"
#include "game/display_screen.h"
#include "game/globals.h"
#include "game/game.h"
#include "game/items.h"
#include "minigames/timingbar.h"
#include "minigames/targetgame.h"
#include "minigames/basketcatch.h"
#include "string.h"

// enums
static enum {
  RUNNING,
  WIN,
  LOSE
} MinigameStatus;

// variables
static bool cookResultApplied = false;
Holding newHolding;
static char panelTitle[9] = "";
int minigameSelection;
const char* debugText;

// function prototypes
static void ReduceItemQuantity(void);

void InitCook() {
  MinigameStatus = false;
  cookResultApplied = false;

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

  minigameSelection = GetRandomValue(0, 2);

  FoodCategory *categories = NULL;
  if (holding.origin == FROM_FRIDGE) {
    categories = allFridge;
  } else if (holding.origin == FROM_PANTRY) {
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
  newHolding = (Holding){
    categoryId, 
    nextVariantId, 
    nextVariant.cook_type,
    holding.origin,   // Keep the original origin
    ARRAY_FOOD        // Food items stay in ARRAY_FOOD
  };
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
    PopScene();
    return;
  } else if (IsKeyPressed(KEY_ENTER)) {
    PopScene();
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
    ReduceItemQuantity();
    TraceLog(LOG_INFO, "item name: %s", debugText);
    cookResultApplied = true;
    PopScene();
  }
  if (MinigameStatus == LOSE) {
    PopScene();
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

static void ReduceItemQuantity(void) {
  if (holding.categoryId < 0) return;

  if (holding.origin == FROM_FRIDGE) {
    for (int i = 0; i < 11; i++) {
      if (stockedFridge[i].categoryId == holding.categoryId) {
        if (stockedFridge[i].quantity > 0) stockedFridge[i].quantity--;
        break;
      }
    }
  } else if (holding.origin == FROM_PANTRY) {
    for (int i = 0; i < 10; i++) {
      if (stockedPantry[i].categoryId == holding.categoryId) {
        if (stockedPantry[i].quantity > 0) stockedPantry[i].quantity--;
        break;
      }
    }
  }
}