#ifndef ITEMS
#define ITEMS

#include "game/cooking.h"

typedef struct {
  int categoryId;
  int variantId;
  COOK_TYPE cookType;
} itemType;

typedef struct {
  const char *name;
  COOK_TYPE cook_type;
  PREP_TYPE prep_type;
  float price;
  const char *filePath;
  int variant;
} Foods;

typedef struct {
  const char *categoryName;
  Foods *variants;
  int variantCount;
} FoodCategory;

typedef struct {
  int categoryId;
  int quantity;
} stockItem;

extern FoodCategory allFoods[];
extern stockItem stockedFridge[];
extern FoodCategory allPantry[];
extern stockItem stockedPantry[];

#endif
