#include "core/enums.h"
#include "external/raylib.h"
#include "external/raygui.h"
#include "game/globals.h"
#include "game/items.h"
#include "game/state.h"
#include "stddef.h"
#include "stdlib.h"

// game state
GameState state = {0};

// cooking
COOK_TYPE currentCookType;
PREP_TYPE currentPrepType;
PREP_TYPE prepFrom;

// inventory
ITEM_ORIGIN currentInventoryType;

// game
Holding holding;
Texture2D playerTexture[4];

// Dynamic item definitions loaded from JSON
FoodCategory *allFridge = NULL;
FoodCategory *allPantry = NULL;
StockItem *stockedFridge = NULL;
StockItem *stockedPantry = NULL;
int fridgeItemCountRuntime = 0;
int pantryItemCountRuntime = 0;
int stockedFridgeCount = 0;
int stockedPantryCount = 0;
Rectangle totalArea = (Rectangle){512-277.2f, 288-142.8f, 554.4f, 285.6f};
Rectangle panelBounds = (Rectangle){512-277.2f, 288-142.8f, 554.4f, 272.8f};
Rectangle notifPanelBounds = (Rectangle){512-140.0f, 40, 280, 40};

// notification system
static const char* notifText = NULL;
static NOTIF_TYPE notifTypeActive = INFO;
static float notifDuration = 0.0f;
const float NOTIF_DISPLAY_TIME = 3.0f;

// notification summoner
void SummonNotif(const char* text, NOTIF_TYPE notifType) {
  notifText = text;
  notifTypeActive = notifType;
  notifDuration = NOTIF_DISPLAY_TIME;
}

// update notifications (call once per frame)
void UpdateNotifications() {
  if (notifDuration > 0) {
    notifDuration -= GetFrameTime();
  } else {
    notifText = NULL;
  }
}

// draw notifications (call in DrawScene)
void DrawNotifications() {
  if (notifText == NULL || notifDuration <= 0) {
    return;
  }

  Color notifColor;

  switch (notifTypeActive) {
    case INFO:
      notifColor = (Color){157, 164, 212, 255};
      break;
    case WARNING:
      notifColor = (Color){156, 175, 136, 255};
      break;
    case ERROR:
      notifColor = (Color){221, 135, 135, 255};
      break;
    case SUCCESS:
      notifColor = (Color){156, 175, 136, 255};
      break;
  }

  DrawRectangle((int)notifPanelBounds.x, (int)notifPanelBounds.y, (int)notifPanelBounds.width, (int)notifPanelBounds.height, notifColor);
  DrawRectangle((int)notifPanelBounds.x + 5, (int)notifPanelBounds.y + 5, (int)notifPanelBounds.width - 10, (int)notifPanelBounds.height - 10, WHITE);
  DrawText(notifText, (int)notifPanelBounds.x + 10, (int)notifPanelBounds.y + (int)notifPanelBounds.height / 2 - 10, 20, notifColor);
}

// Get category array based on holding origin (FROM_FRIDGE or FROM_PANTRY)
FoodCategory* GetHoldingCategories(Holding* holding) {
  if (holding->origin == FROM_FRIDGE) {
    return allFridge;
  } else if (holding->origin == FROM_PANTRY) {
    return allPantry;
  }
  return NULL;
}

int GetCategoryCount(ITEM_ORIGIN origin) {
  if (origin == FROM_FRIDGE) {
    return fridgeItemCountRuntime;
  }
  if (origin == FROM_PANTRY) {
    return pantryItemCountRuntime;
  }
  return 0;
}
