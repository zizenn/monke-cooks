#include "external/raylib.h"
#include "external/raygui.h"
#include "game/cooking.h"
#include "game/display_screen.h"
#include "game/game.h"
#include "game/items.h"
#include "game/texture_cache.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"

// variables
int categoryId;
Foods nextVariant;
int nextVariantId;

// function prototypes
static void Apply();

void PrepFood(int currentTile) {
  FoodCategory* categories = NULL;

  if (itemFrom == FROM_FRIDGE) {
    categories = allFoods;
  } else if (itemFrom == FROM_PANTRY) {
    categories = allPantry;
  } else {
    return;
  }

  categoryId = holding.categoryId;
  int currentVariantId = holding.variantId;
  nextVariantId = currentVariantId + 1;
  int variantCount = categories[categoryId].variantCount;

  if (currentVariantId < 0 || currentVariantId >= variantCount || nextVariantId >= variantCount) {
    return;
  }

  Foods currentVariant = categories[categoryId].variants[currentVariantId];
  nextVariant = categories[categoryId].variants[nextVariantId];

  currentPrepType = currentVariant.prep_type;

  if (currentPrepType == PREP_NONE) {
    return;
  }

  // Enforce location-specific prep constraints
  // Crack only at assembly table (7)
  if (currentPrepType == CRACK && currentTile != 7) {
    return;
  }

  // Wash only at sink (8)
  if (currentPrepType == WASH && currentTile != 8) {
    return;
  }

  // Cut only at cutting station (9)
  if (currentPrepType == CUT && currentTile != 9) {
    return;
  }

  // Ground only at grinding station (12)
  if (currentPrepType == GROUND && currentTile != 12) {
    return;
  }

  Apply();
}

static void Apply() {
  ReleaseTextureArray(playerTexture, 4);
  Texture2D newTexture = AcquireCachedTexture(nextVariant.filePath);
  FillTextureArray(playerTexture, 4, newTexture);

  holding = (ItemType){categoryId, nextVariantId, nextVariant.cook_type};
  currentPrepType = nextVariant.prep_type;
}
