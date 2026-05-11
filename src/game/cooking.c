#include "external/raylib.h"
#include "external/raygui.h"
#include "game/display_screen.h"
#include "game/globals.h"
#include "game/game.h"
#include "game/items.h"
#include "game/state.h"
#include "minigames/timingbar.h"
#include "minigames/targetgame.h"
#include "minigames/basketcatch.h"
#include "string.h"

// function prototypes
static void ReduceItemQuantity(void);

// === MINIGAME HANDLER STRUCT ===
typedef struct {
  void (*init)(void);
  void (*update)(void);
  void (*draw)(void);
  void (*unload)(void);
} MinigameHandler;

// === MINIGAME HANDLERS TABLE ===
static const MinigameHandler minigameHandlers[] = {
  { InitBarMinigame, UpdateBarMinigame, DrawBarMinigame, UnloadBarMinigame },
  { InitTargetMinigame, UpdateTargetMinigame, DrawTargetMinigame, UnloadTargetMinigame },
  { InitBasketMinigame, UpdateBasketMinigame, DrawBasketMinigame, UnloadBasketMinigame }
};

#define MINIGAME_COUNT (sizeof(minigameHandlers) / sizeof(minigameHandlers[0]))

void InitCook() {
  state.cook.minigameStatus = RUNNING;
  state.cook.cookResultApplied = false;

  switch (currentCookType) {
    case PAN:
      strcpy(state.cook.panelTitle, "stove");
      break;
    case DEEP_FRY:
      strcpy(state.cook.panelTitle, "deep fry");
      break;
    case OVEN:
      strcpy(state.cook.panelTitle, "oven");
      break;
    case GRILL:
      strcpy(state.cook.panelTitle, "grill");
      break;
    case 0:
      break;
  }

  state.cook.minigameSelection = GetRandomValue(0, 2);

  FoodCategory *categories = GetHoldingCategories(&holding);
  if (categories == NULL) {
    return;
  }

  int categoryId = holding.categoryId;
  int nextVariantId = holding.variantId + 1;
  int categoryCount = GetCategoryCount(holding.origin);
  if (categoryId < 0 || categoryId >= categoryCount || nextVariantId >= categories[categoryId].variantCount) {
    return;
  }

  Foods nextVariant = categories[categoryId].variants[nextVariantId];
  state.cook.newHolding = (Holding){
    categoryId, 
    nextVariantId, 
    nextVariant.cook_type,
    holding.origin,   // Keep the original origin
    ARRAY_FOOD        // Food items stay in ARRAY_FOOD
  };

  minigameHandlers[state.cook.minigameSelection].init();
}

void UpdateCook() {
  if (IsKeyPressed(KEY_ESCAPE)) {
    PopScene();
    return;
  } else if (IsKeyPressed(KEY_ENTER)) {
    PopScene();
    return;
  }

  minigameHandlers[state.cook.minigameSelection].update();

  switch (state.cook.minigameSelection) {
    case 0:
      if (timingBarResult == TIMING_BAR_WIN) {
        state.cook.minigameStatus = WIN;
      } else if (timingBarResult == TIMING_BAR_LOSE) {
        state.cook.minigameStatus = LOSE;
      } else {
        state.cook.minigameStatus = RUNNING;
      }
      break;
    case 1:
      if (targetGameResult == TARGET_GAME_WIN) {
        state.cook.minigameStatus = WIN;
      } else if (targetGameResult == TARGET_GAME_LOSE) {
        state.cook.minigameStatus = LOSE;
      } else {
        state.cook.minigameStatus = RUNNING;
      }
      break;
    case 2:
      if (basketCatchResult == BASKET_CATCH_WIN) {
        state.cook.minigameStatus = WIN;
      } else if (basketCatchResult == BASKET_CATCH_LOSE) {
        state.cook.minigameStatus = LOSE;
      } else {
        state.cook.minigameStatus = RUNNING;
      }
      break;
  }

  if (state.cook.minigameStatus == WIN && !state.cook.cookResultApplied) {
    // Textures are preloaded and persistent - no need to load/unload
    holding = state.cook.newHolding;
    ReduceItemQuantity();
    state.cook.cookResultApplied = true;
    PopScene();
  }
  if (state.cook.minigameStatus == LOSE) {
    PopScene();
  }
}

void DrawCook() {
  GuiPanel(panelBounds, state.cook.panelTitle);

  minigameHandlers[state.cook.minigameSelection].draw();
}

void UnloadCook() {
  minigameHandlers[state.cook.minigameSelection].unload();
}

static void ReduceItemQuantity(void) {
  if (holding.categoryId < 0) return;

  if (holding.origin == FROM_FRIDGE) {
    for (int i = 0; i < stockedFridgeCount; i++) {
      if (stockedFridge[i].categoryId == holding.categoryId) {
        if (stockedFridge[i].quantity > 0) stockedFridge[i].quantity--;
        break;
      }
    }
  } else if (holding.origin == FROM_PANTRY) {
    for (int i = 0; i < stockedPantryCount; i++) {
      if (stockedPantry[i].categoryId == holding.categoryId) {
        if (stockedPantry[i].quantity > 0) stockedPantry[i].quantity--;
        break;
      }
    }
  }
}
