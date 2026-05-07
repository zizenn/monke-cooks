#ifndef ITEMS_H
#define ITEMS_H

#include "core/types.h"

extern FoodCategory allFridge[];
extern StockItem stockedFridge[];
#define STOCKED_FRIDGE_COUNT 11
extern int fridgeItemCountRuntime;

extern FoodCategory allPantry[];
extern StockItem stockedPantry[];
#define STOCKED_PANTRY_COUNT 10
extern int pantryItemCountRuntime;

void InitializeRuntimeCounts(void);

// Get category array based on holding origin (FROM_FRIDGE or FROM_PANTRY)
FoodCategory* GetHoldingCategories(Holding* holding);

#endif
