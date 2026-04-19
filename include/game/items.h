#ifndef ITEMS
#define ITEMS

#include "external/raylib.h"
#include "game/cooking.h"

typedef struct {
  int foodIndex;
  int quantity;
} stockItem;

typedef struct {
  const char *name;
  COOK_TYPE cook_type;
  float price;
  const char *filePath;
} Foods;

extern Foods allFoods[];
extern stockItem stockedItems[];

#endif
