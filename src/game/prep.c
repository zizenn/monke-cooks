#include "external/raylib.h"
#include "external/raygui.h"
#include "game/cooking.h"
#include "game/screens.h"
#include "game/display_screen.h"
#include "minigames/minigame.h"
#include "game/game.h"
#include "game/globals.h"
#include "game/items.h"
#include "game/texture_cache.h"
#include "stdio.h"
#include "ctype.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"

void PrepFood() {
  FoodCategory* categories = NULL;

  if (itemFrom == FROM_FRIDGE) {
    categories = allFoods;
  } else if (itemFrom == FROM_PANTRY) {
    categories = allPantry;
  } else {
    return;
  }

  int categoryId = holding.categoryId;
  int currentVariantId = holding.variantId;
  int nextVariantId = currentVariantId + 1;
  int variantCount = categories[categoryId].variantCount;

  if (currentVariantId < 0 || currentVariantId >= variantCount || nextVariantId >= variantCount) {
    return;
  }

  Foods currentVariant = categories[categoryId].variants[currentVariantId];
  Foods nextVariant = categories[categoryId].variants[nextVariantId];

  currentPrepType = currentVariant.prep_type;

  if (currentPrepType == PREP_NONE) {
    return;
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

  ReleaseTextureArray(playerTexture, 4);
  Texture2D newTexture = AcquireCachedTexture(nextVariant.filePath);
  FillTextureArray(playerTexture, 4, newTexture);

  holding = (ItemType){categoryId, nextVariantId, nextVariant.cook_type};
  TraceLog(LOG_INFO, "holding: %d = variant id", nextVariantId);
  currentPrepType = nextVariant.prep_type;
}
