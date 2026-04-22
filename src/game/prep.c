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
static const char* newTextureFilePath;
static const char* currentItemName;

void PrepFood() {
  currentCategoryID = holding.categoryId;
  currentVariantID = holding.variantId;

  if (itemFrom == FROM_FRIDGE) {
    newTextureFilePath = allFoods[currentCategoryID].variants[currentVariantID].filePath;
    currentItemName = allFoods[currentCategoryID].variants[currentVariantID].name;
  } else if (itemFrom == FROM_PANTRY) {
    newTextureFilePath = allPantry[currentCategoryID].variants[currentVariantID].filePath;
    currentItemName = allPantry[currentCategoryID].variants[currentVariantID].name;
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
}
