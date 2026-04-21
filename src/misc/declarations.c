#include "external/raylib.h"
#include "external/raygui.h"
#include "game/globals.h"
#include "game/items.h"
#include "game/cooking.h"
#include "minigames/minigame.h"
#include "stdint.h"
#include "stddef.h"

// game
HoldingItem holding;

// ===== INGREDIENT VARIANT ARRAYS =====

// EGGS: Raw can't be cooked directly, must be cracked first
Foods eggVariants[] = {
  { "RAW_EGG", 0, 3.50, "assets/food/ingredients/eggs/egg_orange.png" },
  { "CRACKED_EGG", PAN, 3.50, "assets/food/ingredients/eggs/egg_orange_cracked.png" },
  { "FRIED_EGG", 0, 3.50, "assets/food/ingredients/eggs/egg_orange.png" },
};

// RICE: Raw can't be cooked, must go through intermediate states
Foods riceVariants[] = {
  { "RAW_RICE", 0, 5.00, "assets/food/ingredients/grains/rice.png" },
  { "WASHED_RICE", OVEN, 5.00, "assets/food/ingredients/grains/rice.png" },
  { "COOKED_RICE", 0, 6.50, "assets/food/ingredients/grains/rice.png" },
};

// MUSHROOMS: Raw must be sliced/prepared before cooking
Foods shiitakeVariants[] = {
  { "RAW_SHIITAKE", 0, 5.50, "assets/food/ingredients/mushrooms/shiitake.png" },
  { "SLICED_SHIITAKE", PAN, 5.50, "assets/food/ingredients/mushrooms/hen_of_the_woods_sliced.png" },
  { "COOKED_SHIITAKE", 0, 7.00, "assets/food/ingredients/mushrooms/shiitake.png" },
};

// CHICKPEAS: Raw can't be cooked, must be soaked/prepared
Foods chickpeaVariants[] = {
  { "RAW_CHICKPEA", 0, 8.00, "assets/food/ingredients/beans/chickpea.png" },
  { "SOAKED_CHICKPEA", OVEN, 8.00, "assets/food/ingredients/beans/chickpea.png" },
  { "COOKED_CHICKPEA", 0, 10.00, "assets/food/ingredients/beans/chickpea.png" },
};

// LENTILS: Raw needs preparation before cooking
Foods lentilVariants[] = {
  { "RAW_LENTIL", 0, 6.00, "assets/food/ingredients/beans/lentil.png" },
  { "RINSED_LENTIL", OVEN, 6.00, "assets/food/ingredients/beans/lentil.png" },
  { "COOKED_LENTIL", 0, 8.00, "assets/food/ingredients/beans/lentil.png" },
};

// OYSTER MUSHROOMS: Raw, sliced, then cooked
Foods oysterMushroomVariants[] = {
  { "RAW_OYSTER_MUSHROOM", 0, 7.00, "assets/food/ingredients/mushrooms/oyster_mushroom.png" },
  { "SLICED_OYSTER", PAN, 7.00, "assets/food/ingredients/mushrooms/oyster_mushroom.png" },
  { "COOKED_OYSTER", 0, 9.00, "assets/food/ingredients/mushrooms/oyster_mushroom.png" },
};

// MOZZARELLA: Raw cheese, can be melted
Foods mozzarellaVariants[] = {
  { "FRESH_MOZZARELLA", 0, 12.00, "assets/food/ingredients/cheese/mozzarella.png" },
  { "MELTED_MOZZARELLA", PAN, 12.00, "assets/food/ingredients/cheese/mozzarella.png" },
};

// PARMIGIANO: Raw cheese, can be melted
Foods parmiganoVariants[] = {
  { "FRESH_PARMIGIANO", 0, 14.00, "assets/food/ingredients/cheese/parmigiano_reggiano.png" },
  { "MELTED_PARMIGIANO", PAN, 14.00, "assets/food/ingredients/cheese/parmigiano_reggiano.png" },
};

// CHEDDAR: Raw cheese, can be melted
Foods cheddarVariants[] = {
  { "FRESH_CHEDDAR", 0, 10.00, "assets/food/ingredients/cheese/cheddar.png" },
  { "MELTED_CHEDDAR", PAN, 10.00, "assets/food/ingredients/cheese/cheddar.png" },
};

// SPICES (PEPPER): Can be mixed/used raw or heated
Foods pepperVariants[] = {
  { "RAW_PEPPER", 0, 2.50, "assets/food/ingredients/spices/pepper.png" },
  { "GROUND_PEPPER", PAN, 2.50, "assets/food/ingredients/spices/pepper.png" },
};

// CUMIN: Can be dry roasted
Foods cuminVariants[] = {
  { "RAW_CUMIN", 0, 3.00, "assets/food/ingredients/spices/cumin.png" },
  { "ROASTED_CUMIN", PAN, 3.00, "assets/food/ingredients/spices/cumin.png" },
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

stockItemIngredient stockedIngredients[] = {
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
  { "RAW_TURMERIC", 0, 4.50, "assets/food/ingredients/spices/turmeric.png" },
  { "GROUND_TURMERIC", PAN, 4.50, "assets/food/ingredients/spices/turmeric.png" },
};

// VANILLA: Flavoring that can be raw or processed
Foods vanillaVariants[] = {
  { "RAW_VANILLA", 0, 8.00, "assets/food/ingredients/spices/vanilla.png" },
  { "PROCESSED_VANILLA", PAN, 8.00, "assets/food/ingredients/spices/vanilla.png" },
};

// STAR ANISE: Spice that can be whole or ground
Foods starAniseVariants[] = {
  { "WHOLE_STAR_ANISE", 0, 5.00, "assets/food/ingredients/spices/star_anise.png" },
  { "GROUND_STAR_ANISE", PAN, 5.00, "assets/food/ingredients/spices/star_anise.png" },
};

// CLOVES: Spice that can be whole or ground
Foods clovesVariants[] = {
  { "WHOLE_CLOVES", 0, 6.00, "assets/food/ingredients/spices/cloves.png" },
  { "GROUND_CLOVES", PAN, 6.00, "assets/food/ingredients/spices/cloves.png" },
};

// NUTMEG: Spice that can be whole or ground
Foods nutmegVariants[] = {
  { "WHOLE_NUTMEG", 0, 5.50, "assets/food/ingredients/spices/nutmeg.png" },
  { "GROUND_NUTMEG", PAN, 5.50, "assets/food/ingredients/spices/nutmeg.png" },
};

// WHEAT: Grain that can be raw or processed
Foods wheatVariants[] = {
  { "RAW_WHEAT", 0, 3.00, "assets/food/ingredients/grains/wheat.png" },
  { "MILLED_WHEAT", OVEN, 3.00, "assets/food/ingredients/grains/wheat.png" },
};

// BARLEY: Grain for cooking
Foods barleyVariants[] = {
  { "RAW_BARLEY", 0, 3.50, "assets/food/ingredients/grains/barley.png" },
  { "COOKED_BARLEY", OVEN, 3.50, "assets/food/ingredients/grains/barley.png" },
};

// NORI: Seaweed for sushi
Foods noriVariants[] = {
  { "DRIED_NORI", 0, 7.00, "assets/food/ingredients/seaweeds/nori_toasted.png" },
  { "TOASTED_NORI", PAN, 7.00, "assets/food/ingredients/seaweeds/nori_toasted.png" },
};

// WAKAME: Seaweed for soups
Foods wakameVariants[] = {
  { "DRIED_WAKAME", 0, 6.00, "assets/food/ingredients/seaweeds/wakame.png" },
  { "SOAKED_WAKAME", OVEN, 6.00, "assets/food/ingredients/seaweeds/wakame.png" },
};

// MILK: Dairy for cooking
Foods milkVariants[] = {
  { "FRESH_MILK", 0, 4.00, "assets/food/ingredients/yogurt_and_milk/milk_bottled.png" },
  { "HEATED_MILK", OVEN, 4.00, "assets/food/ingredients/yogurt_and_milk/milk_bottled.png" },
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

stockItemIngredient stockedPantry[] = {
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

