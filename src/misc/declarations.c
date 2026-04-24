#include "external/raylib.h"
#include "external/raygui.h"
#include "game/game.h"
#include "game/globals.h"
#include "game/items.h"
#include "game/cooking.h"
#include "minigames/minigame.h"
#include "stdint.h"
#include "stddef.h"

// cooking
COOK_TYPE currentCookType;
PREP_TYPE currentPrepType;
PREP_TYPE prepFrom;

// game
ItemType holding;
whereIsItemFrom itemFrom;
Texture2D playerTexture[4];

// ===== INGREDIENT VARIANT ARRAYS =====

// EGGS: Raw can't be cooked directly, must be cracked first
Foods eggVariants[] = {
  { "RAW_EGG", 0, CRACK, 3.50, "assets/food/ingredients/eggs/egg_orange.png", 1 },
  { "CRACKED_EGG", PAN, PREP_NONE, 3.50, "assets/food/ingredients/eggs/egg_orange_cracked.png", 2 },
  { "FRIED_EGG", 0, PREP_NONE, 3.50, "assets/food/dishes/38_friedegg.png", 3 },
};

// RICE: Raw can't be cooked, must go through intermediate states
Foods riceVariants[] = {
  { "RAW_RICE", 0, WASH, 5.00, "assets/food/ingredients/grains/rice.png", 1 },
  { "WASHED_RICE", OVEN, PREP_NONE, 5.00, "assets/food/ingredients/grains/rice.png", 2 },
  { "COOKED_RICE", 0, PREP_NONE, 6.50, "assets/food/ingredients/grains/rice.png", 3 },
};

// MUSHROOMS: Raw must be sliced/prepared before cooking
Foods shiitakeVariants[] = {
  { "RAW_SHIITAKE", 0, CUT, 5.50, "assets/food/ingredients/mushrooms/shiitake.png", 1 },
  { "SLICED_SHIITAKE", PAN, PREP_NONE, 5.50, "assets/food/ingredients/mushrooms/hen_of_the_woods_sliced.png", 2 },
  { "COOKED_SHIITAKE", 0, PREP_NONE, 7.00, "assets/food/ingredients/mushrooms/shiitake.png", 3 },
};

// CHICKPEAS: Raw can't be cooked, must be soaked/prepared
Foods chickpeaVariants[] = {
  { "RAW_CHICKPEA", 0, WASH, 8.00, "assets/food/ingredients/beans/chickpea.png", 1 },
  { "SOAKED_CHICKPEA", OVEN, PREP_NONE, 8.00, "assets/food/ingredients/beans/chickpea.png", 2 },
  { "COOKED_CHICKPEA", 0, PREP_NONE, 10.00, "assets/food/ingredients/beans/chickpea.png", 3 },
};

// LENTILS: Raw needs preparation before cooking
Foods lentilVariants[] = {
  { "RAW_LENTIL", 0, WASH, 6.00, "assets/food/ingredients/beans/lentil.png", 1 },
  { "RINSED_LENTIL", OVEN, PREP_NONE, 6.00, "assets/food/ingredients/beans/lentil.png", 2 },
  { "COOKED_LENTIL", 0, PREP_NONE, 8.00, "assets/food/ingredients/beans/lentil.png", 3 },
};

// OYSTER MUSHROOMS: Raw, sliced, then cooked
Foods oysterMushroomVariants[] = {
  { "RAW_OYSTER_MUSHROOM", 0, CUT, 7.00, "assets/food/ingredients/mushrooms/oyster_mushroom.png", 1 },
  { "SLICED_OYSTER", PAN, PREP_NONE, 7.00, "assets/food/ingredients/mushrooms/oyster_mushroom.png", 2 },
  { "COOKED_OYSTER", 0, PREP_NONE, 9.00, "assets/food/ingredients/mushrooms/oyster_mushroom.png", 3 },
};

// MOZZARELLA: Raw cheese, can be melted
Foods mozzarellaVariants[] = {
  { "FRESH_MOZZARELLA", PAN, CUT, 12.00, "assets/food/ingredients/cheese/mozzarella.png", 1 },
  { "MELTED_MOZZARELLA", 0, PREP_NONE, 12.00, "assets/food/ingredients/cheese/mozzarella.png", 2 },
};

// PARMIGIANO: Raw cheese, can be melted
Foods parmiganoVariants[] = {
  { "FRESH_PARMIGIANO", PAN, CUT, 14.00, "assets/food/ingredients/cheese/parmigiano_reggiano.png", 1 },
  { "MELTED_PARMIGIANO", 0, PREP_NONE, 14.00, "assets/food/ingredients/cheese/parmigiano_reggiano.png", 2 },
};

// CHEDDAR: Raw cheese, can be melted
Foods cheddarVariants[] = {
  { "FRESH_CHEDDAR", PAN, CUT, 10.00, "assets/food/ingredients/cheese/cheddar.png", 1 },
  { "MELTED_CHEDDAR", 0, PREP_NONE, 10.00, "assets/food/ingredients/cheese/cheddar.png", 2 },
};

// SPICES (PEPPER): Can be mixed/used raw or heated
Foods pepperVariants[] = {
  { "RAW_PEPPER", PAN, GROUND, 2.50, "assets/food/ingredients/spices/pepper.png", 1 },
  { "GROUND_PEPPER", 0, PREP_NONE, 2.50, "assets/food/ingredients/spices/pepper.png", 2 },
};

// CUMIN: Can be dry roasted
Foods cuminVariants[] = {
  { "RAW_CUMIN", PAN, CUT, 3.00, "assets/food/ingredients/spices/cumin.png", 1 },
  { "ROASTED_CUMIN", 0, PREP_NONE, 3.00, "assets/food/ingredients/spices/cumin.png", 2 },
};

// All food categories with variants
FoodCategory allFoods[] = {
  { "EGG", eggVariants, 3 },
  { "RICE", riceVariants, 3 },
  { "SHIITAKE", shiitakeVariants, 3 },
  { "CHICKPEA", chickpeaVariants, 3 },
  { "LENTIL", lentilVariants, 3 },
  { "OYSTER_MUSHROOM", oysterMushroomVariants, 3 },
  { "MOZZARELLA", mozzarellaVariants, 2 },
  { "PARMIGIANO", parmiganoVariants, 2 },
  { "CHEDDAR", cheddarVariants, 2 },
  { "PEPPER", pepperVariants, 2 },
  { "CUMIN", cuminVariants, 2 },
};

StockItem stockedFridge[] = {
  { 0, 2 },   // 2x EGG (category 0)
  { 1, 1 },   // 1x RICE (category 1)
  { 2, 3 },   // 3x SHIITAKE (category 2)
  { 3, 2 },   // 2x CHICKPEA (category 3)
  { 4, 1 },   // 1x LENTIL (category 4)
  { 5, 2 },   // 2x OYSTER_MUSHROOM (category 5)
  { 6, 2 },   // 2x MOZZARELLA (category 6)
  { 7, 1 },   // 1x PARMIGIANO (category 7)
  { 8, 3 },   // 3x CHEDDAR (category 8)
  { 9, 4 },   // 4x PEPPER (category 9)
  { 10, 2 },  // 2x CUMIN (category 10)
};

// ===== PANTRY ITEMS =====

// TURMERIC: Spice that can be used raw or heated
Foods turmericVariants[] = {
  { "RAW_TURMERIC", PAN, GROUND, 4.50, "assets/food/ingredients/spices/turmeric.png", 1 },
  { "GROUND_TURMERIC", 0, PREP_NONE, 4.50, "assets/food/ingredients/spices/turmeric.png", 2 },
};

// VANILLA: Flavoring that can be raw or processed
Foods vanillaVariants[] = {
  { "RAW_VANILLA", PAN, CUT, 8.00, "assets/food/ingredients/spices/vanilla.png", 1 },
  { "PROCESSED_VANILLA", 0, PREP_NONE, 8.00, "assets/food/ingredients/spices/vanilla.png", 2 },
};

// STAR ANISE: Spice that can be whole or ground
Foods starAniseVariants[] = {
  { "WHOLE_STAR_ANISE", PAN, GROUND, 5.00, "assets/food/ingredients/spices/star_anise.png", 1 },
  { "GROUND_STAR_ANISE", 0, PREP_NONE, 5.00, "assets/food/ingredients/spices/star_anise.png", 2 },
};

// CLOVES: Spice that can be whole or ground
Foods clovesVariants[] = {
  { "WHOLE_CLOVES", PAN, GROUND, 6.00, "assets/food/ingredients/spices/cloves.png", 1 },
  { "GROUND_CLOVES", 0, PREP_NONE, 6.00, "assets/food/ingredients/spices/cloves.png", 2 },
};

// NUTMEG: Spice that can be whole or ground
Foods nutmegVariants[] = {
  { "WHOLE_NUTMEG", PAN, GROUND, 5.50, "assets/food/ingredients/spices/nutmeg.png", 1 },
  { "GROUND_NUTMEG", 0, PREP_NONE, 5.50, "assets/food/ingredients/spices/nutmeg.png", 2 },
};

// WHEAT: Grain that can be raw or processed
Foods wheatVariants[] = {
  { "RAW_WHEAT", OVEN, WASH, 3.00, "assets/food/ingredients/grains/wheat.png", 1 },
  { "MILLED_WHEAT", 0, PREP_NONE, 3.00, "assets/food/ingredients/grains/wheat.png", 2 },
};

// BARLEY: Grain for cooking
Foods barleyVariants[] = {
  { "RAW_BARLEY", OVEN, WASH, 3.50, "assets/food/ingredients/grains/barley.png", 1 },
  { "COOKED_BARLEY", 0, PREP_NONE, 3.50, "assets/food/ingredients/grains/barley.png", 2 },
};

// NORI: Seaweed for sushi
Foods noriVariants[] = {
  { "DRIED_NORI", PAN, PREP_NONE, 7.00, "assets/food/ingredients/seaweeds/nori_toasted.png", 1 },
  { "TOASTED_NORI", 0, PREP_NONE, 7.00, "assets/food/ingredients/seaweeds/nori_toasted.png", 2 },
};

// WAKAME: Seaweed for soups
Foods wakameVariants[] = {
  { "DRIED_WAKAME", 0, WASH, 6.00, "assets/food/ingredients/seaweeds/wakame.png", 1 },
  { "SOAKED_WAKAME", OVEN, PREP_NONE, 6.00, "assets/food/ingredients/seaweeds/wakame.png", 2 },
};

// MILK: Dairy for cooking
Foods milkVariants[] = {
  { "FRESH_MILK", OVEN, PREP_NONE, 4.00, "assets/food/ingredients/yogurt_and_milk/milk_bottled.png", 1 },
  { "HEATED_MILK", 0, PREP_NONE, 4.00, "assets/food/ingredients/yogurt_and_milk/milk_bottled.png", 2 },
};

// All pantry categories
FoodCategory allPantry[] = {
  { "TURMERIC", turmericVariants, 2 },
  { "VANILLA", vanillaVariants, 2 },
  { "STAR_ANISE", starAniseVariants, 2 },
  { "CLOVES", clovesVariants, 2 },
  { "NUTMEG", nutmegVariants, 2 },
  { "WHEAT", wheatVariants, 2 },
  { "BARLEY", barleyVariants, 2 },
  { "NORI", noriVariants, 2 },
  { "WAKAME", wakameVariants, 2 },
  { "MILK", milkVariants, 2 },
};

StockItem stockedPantry[] = {
  { 0, 3 },   // 3x TURMERIC (category 0)
  { 1, 1 },   // 1x VANILLA (category 1)
  { 2, 2 },   // 2x STAR_ANISE (category 2)
  { 3, 2 },   // 2x CLOVES (category 3)
  { 4, 1 },   // 1x NUTMEG (category 4)
  { 5, 4 },   // 4x WHEAT (category 5)
  { 6, 2 },   // 2x BARLEY (category 6)
  { 7, 3 },   // 3x NORI (category 7)
  { 8, 2 },   // 2x WAKAME (category 8)
  { 9, 2 },   // 2x MILK (category 9)
};
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
