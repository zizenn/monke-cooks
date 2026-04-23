#include "external/raylib.h"
#include "external/raygui.h"
#include "game/cooking.h"
#include "game/screens.h"
#include "game/display_screen.h"
#include "minigames/minigame.h"
#include "game/game.h"
#include "game/globals.h"
#include "game/items.h"
#include "stdio.h"
#include "ctype.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"

static int currentCategoryID;
static int currentVariantID;
static COOK_TYPE newCookType;
static const char* newTextureFilePath;
static const char* currentItemName;
static int currentItemVariant;

void PrepFood() {
  currentCategoryID = holding.categoryId;
  currentVariantID = holding.variantId;

  if (itemFrom == FROM_FRIDGE) {
    newTextureFilePath = allFoods[currentCategoryID].variants[currentVariantID++].filePath;
    currentItemName = allFoods[currentCategoryID].variants[currentVariantID].name;
    currentPrepType = allFoods[currentCategoryID].variants[currentVariantID].prep_type;
    currentItemVariant = allFoods[currentCategoryID].variants[currentVariantID].variant;
    newCookType = allFoods[currentCategoryID].variants[currentVariantID].cook_type;
  } else if (itemFrom == FROM_PANTRY) {
    newTextureFilePath = allPantry[currentCategoryID].variants[currentVariantID++].filePath;
    currentItemName = allPantry[currentCategoryID].variants[currentVariantID].name;
    currentPrepType = allPantry[currentCategoryID].variants[currentVariantID].prep_type;
    currentItemVariant = allPantry[currentCategoryID].variants[currentVariantID].variant;
    newCookType = allPantry[currentCategoryID].variants[currentVariantID].cook_type;
  }

  // actually prepping the food
  switch (currentPrepType) {
    case PREP_NONE:
      break;
    case CRACK:
      break;
    case WASH:
      break;
    case CUT:
      break;
    case GROUND:
      break;
  }

  // unloading and assigning textures
  for (int i = 0; i < 4; i++) {
    UnloadTexture(playerTexture[i]);
    playerTexture[i] = LoadTexture(newTextureFilePath);
  }

  holding = (ItemType){currentCategoryID, currentVariantID++, newCookType};
}
