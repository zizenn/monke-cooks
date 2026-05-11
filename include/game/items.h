#ifndef ITEMS_H
#define ITEMS_H

#include "core/types.h"
#include "stdbool.h"

// All category definitions
extern FoodCategory *allFridge;
extern FoodCategory *allPantry;

// Stocked item collections (dynamic): these arrays can change at runtime
extern StockItem *stockedFridge;
extern int stockedFridgeCount;

extern StockItem *stockedPantry;
extern int stockedPantryCount;

// Number of categories (calculated at startup)
extern int fridgeItemCountRuntime;
extern int pantryItemCountRuntime;

bool LoadItemData(const char *ingredientsPath, const char *fridgeStockPath, const char *pantryStockPath);
void UnloadItemData(void);
int GetCategoryCount(ITEM_ORIGIN origin);

// Get category array based on holding origin (FROM_FRIDGE or FROM_PANTRY)
FoodCategory* GetHoldingCategories(Holding* holding);

#endif
