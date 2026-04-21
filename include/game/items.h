#ifndef ITEMS
#define ITEMS

#include "external/raylib.h"
#include "game/cooking.h"

typedef struct {
  int categoryId;
  int variantId;
  COOK_TYPE cookType;
} HoldingItem;

typedef struct {
  const char *name;
  COOK_TYPE cook_type;
  float price;
  const char *filePath;
} Foods;

typedef struct {
  const char *categoryName;
  Foods *variants;
  int variantCount;
} FoodCategory;

typedef struct {
  int categoryId;
  int quantity;
} stockItemIngredient;

typedef struct {
  int categoryId;
  int variantId;
  int quantity;
} stockItemFood;

extern FoodCategory allFoods[];
extern stockItemIngredient stockedIngredients[];
extern FoodCategory allPantry[];
extern stockItemIngredient stockedPantry[];

#endif
