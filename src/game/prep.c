#include "external/raylib.h"
#include "external/raygui.h"
#include "game/globals.h"
#include "game/display_screen.h"
#include "game/game.h"
#include "core/enums.h"
#include "game/items.h"
#include "game/state.h"
#include "stdbool.h"
#include "string.h"

// function prototypes
static void Apply();

void PrepFood(int currentTile) {
  GameState *state = GetGameState();
  FoodCategory* categories = GetHoldingCategories(&holding);
  
  if (categories == NULL) {
    return;
  }

  state->prep.categoryId = holding.categoryId;
  int currentVariantId = holding.variantId;
  state->prep.nextVariantId = currentVariantId + 1;
  int variantCount = categories[state->prep.categoryId].variantCount;

  if (currentVariantId < 0 || currentVariantId >= variantCount || state->prep.nextVariantId >= variantCount) {
    return;
  }

  Foods currentVariant = categories[state->prep.categoryId].variants[currentVariantId];
  state->prep.nextVariant = categories[state->prep.categoryId].variants[state->prep.nextVariantId];

  currentPrepType = currentVariant.prep_type;

  if (currentPrepType == PREP_NONE) {
    return;
  }

  // prep only at corresponding areas
  if (currentPrepType == CRACK && currentTile != ASSEMBLY) {
    return;
  }

  if (currentPrepType == WASH && currentTile != SINK) {
    return;
  }

  if (currentPrepType == CUT && currentTile != CUTTING_STATION) {
    return;
  }

  if (currentPrepType == GROUND && currentTile != GRINDING_STATION) {
    return;
  }

  Apply();
}

static void Apply() {
  GameState *state = GetGameState();
  holding = (Holding){
    state->prep.categoryId, 
    state->prep.nextVariantId, 
    state->prep.nextVariant.cook_type,
    holding.origin,   // Keep the original origin
    ARRAY_FOOD        // Food items stay in ARRAY_FOOD
  };
  currentPrepType = state->prep.nextVariant.prep_type;
  const char* debugText = state->prep.nextVariant.name;
  TraceLog(LOG_INFO, "item name: %s", debugText);
}
