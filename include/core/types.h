#ifndef CORE_TYPES_H
#define CORE_TYPES_H

#include "core/enums.h"

// Unified item type - the player's current holding
typedef struct {
  int categoryId;      // index into the array
  int variantId;       // which variant/texture within that item
  COOK_TYPE cookType;  // for cooking minigames
  ItemOrigin origin;   // FROM_FRIDGE or FROM_PANTRY
  ItemArray array;     // which array/collection to look in
} Holding;

// Food ingredient definition
typedef struct {
  const char *name;
  COOK_TYPE cook_type;
  PREP_TYPE prep_type;
  float price;
  int variant;
} Foods;

// Food category with variants
typedef struct {
  const char *categoryName;
  Foods *variants;
  int variantCount;
} FoodCategory;

// Stock tracking
typedef struct {
  int categoryId;
  int quantity;
} StockItem;

#endif
