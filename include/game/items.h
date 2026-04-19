#ifndef ITEMS
#define ITEMS

#include "external/raylib.h"
#include "game/cooking.h"

typedef enum {
  RAW_EGG,
  RAW_RICE,
  RAW_SHIITAKE,
  EMPTY = -1
} ITEM;

typedef struct {
  int foodIndex;
  int quantity;
} stockItemIngredient;

typedef struct {
  int foodIndex;
  int quantity;
} stockItemFood;

typedef struct {
  const char *name;
  COOK_TYPE cook_type;
  float price;
  const char *filePath;
} Foods;

extern Foods allFoods[];
extern Foods allIngredients[];
extern stockItemIngredient stockedIngredients[];

#endif
