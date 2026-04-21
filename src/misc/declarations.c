#include "external/raylib.h"
#include "external/raygui.h"
#include "game/globals.h"
#include "game/items.h"
#include "game/cooking.h"
#include "minigames/minigame.h"
#include "stdint.h"
#include "stddef.h"

// all rectangles
Rectangle totalArea = (Rectangle){640-346.5, 360-178.5, 693, 357};
Rectangle panelBounds = (Rectangle){640-346.5, 360-178.5, 693, 341};
Rectangle notifPanelBounds = (Rectangle){640-175, 50, 350, 50};

// notification system
static const char* notifText = NULL;
static NOTIF_TYPE notifTypeActive = INFO;
static float notifDuration = 0.0f;
const float NOTIF_DISPLAY_TIME = 3.0f;

// notification summoner
void summonNotif(const char* text, NOTIF_TYPE notifType) {
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

Foods allIngredients[] = {
  { "RAW_EGG", PAN, 3.50, "assets/food/ingredients/eggs/egg_orange.png" },
  { "RAW_RICE", OVEN, 5.00, "assets/food/ingredients/grains/rice.png" },
  { "RAW_SHIITAKE_MUSHROOM", PAN, 5.50, "assets/food/ingredients/mushrooms/shiitake.png" },
};

Foods allFoods[] = {

};
