#include "game/items.h"
#include "game/texture_manager.h"
#include "external/parson.h"
#include "external/raylib.h"
#include "stdlib.h"
#include "string.h"

typedef struct {
  FoodCategory *categories;
  int count;
} CategoryLoadResult;

static char *CopyString(const char *source) {
  if (source == NULL) {
    return NULL;
  }
  size_t len = strlen(source);
  char *copy = (char *)malloc(len + 1);
  if (copy == NULL) {
    return NULL;
  }
  memcpy(copy, source, len + 1);
  return copy;
}

static void FreeCategories(FoodCategory *categories, int count) {
  if (categories == NULL) {
    return;
  }

  for (int i = 0; i < count; i++) {
    if (categories[i].variants != NULL) {
      for (int j = 0; j < categories[i].variantCount; j++) {
        free((void *)categories[i].variants[j].name);
      }
      free(categories[i].variants);
    }
    free((void *)categories[i].categoryName);
  }

  free(categories);
}

static int FindCategoryIndexByName(const FoodCategory *categories, int categoryCount, const char *name) {
  if (categories == NULL || name == NULL) {
    return -1;
  }

  for (int i = 0; i < categoryCount; i++) {
    if (categories[i].categoryName != NULL && strcmp(categories[i].categoryName, name) == 0) {
      return i;
    }
  }

  return -1;
}

static bool ParseCookType(const char *value, COOK_TYPE *outType) {
  if (value == NULL || outType == NULL) {
    return false;
  }
  if (strcmp(value, "COOK_NONE") == 0) {
    *outType = COOK_NONE;
    return true;
  }
  if (strcmp(value, "PAN") == 0) {
    *outType = PAN;
    return true;
  }
  if (strcmp(value, "DEEP_FRY") == 0) {
    *outType = DEEP_FRY;
    return true;
  }
  if (strcmp(value, "OVEN") == 0) {
    *outType = OVEN;
    return true;
  }
  if (strcmp(value, "GRILL") == 0) {
    *outType = GRILL;
    return true;
  }
  return false;
}

static bool ParsePrepType(const char *value, PREP_TYPE *outType) {
  if (value == NULL || outType == NULL) {
    return false;
  }
  if (strcmp(value, "PREP_NONE") == 0) {
    *outType = PREP_NONE;
    return true;
  }
  if (strcmp(value, "CRACK") == 0) {
    *outType = CRACK;
    return true;
  }
  if (strcmp(value, "WASH") == 0) {
    *outType = WASH;
    return true;
  }
  if (strcmp(value, "CUT") == 0) {
    *outType = CUT;
    return true;
  }
  if (strcmp(value, "GROUND") == 0) {
    *outType = GROUND;
    return true;
  }
  return false;
}

static bool ParseTextureId(const char *value, int *outTextureId) {
  if (value == NULL || outTextureId == NULL) {
    return false;
  }

  for (int i = 0; i < TEXTURE_COUNT; i++) {
    if (allTextures[i].name != NULL && strcmp(allTextures[i].name, value) == 0) {
      *outTextureId = i;
      return true;
    }
  }

  return false;
}

static bool LoadCategories(JSON_Array *array, CategoryLoadResult *result) {
  if (array == NULL || result == NULL) {
    return false;
  }

  size_t categoryCount = json_array_get_count(array);
  FoodCategory *categories = NULL;

  if (categoryCount > 0) {
    categories = (FoodCategory *)calloc(categoryCount, sizeof(FoodCategory));
    if (categories == NULL) {
      return false;
    }
  }

  for (size_t i = 0; i < categoryCount; i++) {
    JSON_Object *categoryObj = json_array_get_object(array, i);
    if (categoryObj == NULL) {
      FreeCategories(categories, (int)i);
      return false;
    }

    const char *categoryName = json_object_get_string(categoryObj, "category");
    JSON_Array *variantsArray = json_object_get_array(categoryObj, "variants");
    if (categoryName == NULL || variantsArray == NULL) {
      FreeCategories(categories, (int)i);
      return false;
    }

    categories[i].categoryName = CopyString(categoryName);
    if (categories[i].categoryName == NULL) {
      FreeCategories(categories, (int)i + 1);
      return false;
    }

    size_t variantCount = json_array_get_count(variantsArray);
    categories[i].variantCount = (int)variantCount;
    if (variantCount > 0) {
      categories[i].variants = (Foods *)calloc(variantCount, sizeof(Foods));
      if (categories[i].variants == NULL) {
        FreeCategories(categories, (int)i + 1);
        return false;
      }
    }

    for (size_t j = 0; j < variantCount; j++) {
      JSON_Object *variantObj = json_array_get_object(variantsArray, j);
      if (variantObj == NULL) {
        FreeCategories(categories, (int)i + 1);
        return false;
      }

      const char *name = json_object_get_string(variantObj, "name");
      const char *cookType = json_object_get_string(variantObj, "cook_type");
      const char *prepType = json_object_get_string(variantObj, "prep_type");
      const char *textureName = json_object_get_string(variantObj, "texture");
      double price = json_object_get_number(variantObj, "price");
      double variantNum = json_object_get_number(variantObj, "variant");

      COOK_TYPE parsedCookType = COOK_NONE;
      PREP_TYPE parsedPrepType = PREP_NONE;
      int textureId = 0;

      if (name == NULL ||
          !ParseCookType(cookType, &parsedCookType) ||
          !ParsePrepType(prepType, &parsedPrepType) ||
          !ParseTextureId(textureName, &textureId)) {
        FreeCategories(categories, (int)i + 1);
        return false;
      }

      categories[i].variants[j].name = CopyString(name);
      if (categories[i].variants[j].name == NULL) {
        FreeCategories(categories, (int)i + 1);
        return false;
      }

      categories[i].variants[j].cook_type = parsedCookType;
      categories[i].variants[j].prep_type = parsedPrepType;
      categories[i].variants[j].price = (float)price;
      categories[i].variants[j].variant = (int)variantNum;
      categories[i].variants[j].textureId = textureId;
    }
  }

  result->categories = categories;
  result->count = (int)categoryCount;
  return true;
}

static bool LoadStock(const char *path, const FoodCategory *categories, int categoryCount, StockItem **outStock, int *outCount) {
  JSON_Value *rootValue = json_parse_file(path);
  if (rootValue == NULL) {
    TraceLog(LOG_ERROR, "Could not parse stock file: %s", path);
    return false;
  }

  JSON_Object *rootObject = json_value_get_object(rootValue);
  JSON_Array *stockArray = (rootObject != NULL) ? json_object_get_array(rootObject, "stock") : NULL;
  if (stockArray == NULL) {
    json_value_free(rootValue);
    TraceLog(LOG_ERROR, "Stock file missing 'stock' array: %s", path);
    return false;
  }

  size_t sourceCount = json_array_get_count(stockArray);
  StockItem *stock = NULL;
  if (sourceCount > 0) {
    stock = (StockItem *)calloc(sourceCount, sizeof(StockItem));
    if (stock == NULL) {
      json_value_free(rootValue);
      return false;
    }
  }

  int usedCount = 0;
  for (size_t i = 0; i < sourceCount; i++) {
    JSON_Object *stockObj = json_array_get_object(stockArray, i);
    if (stockObj == NULL) {
      free(stock);
      json_value_free(rootValue);
      return false;
    }

    const char *categoryName = json_object_get_string(stockObj, "category");
    double quantityNum = json_object_get_number(stockObj, "quantity");
    int quantity = (int)quantityNum;

    int categoryId = FindCategoryIndexByName(categories, categoryCount, categoryName);
    if (categoryName == NULL || categoryId < 0 || quantity < 0) {
      free(stock);
      json_value_free(rootValue);
      TraceLog(LOG_ERROR, "Invalid stock entry in: %s", path);
      return false;
    }

    int existingIndex = -1;
    for (int j = 0; j < usedCount; j++) {
      if (stock[j].categoryId == categoryId) {
        existingIndex = j;
        break;
      }
    }

    if (existingIndex >= 0) {
      stock[existingIndex].quantity += quantity;
    } else {
      stock[usedCount].categoryId = categoryId;
      stock[usedCount].quantity = quantity;
      usedCount++;
    }
  }

  json_value_free(rootValue);
  *outStock = stock;
  *outCount = usedCount;
  return true;
}

bool LoadItemData(const char *ingredientsPath, const char *fridgeStockPath, const char *pantryStockPath) {
  CategoryLoadResult fridgeCategories = {0};
  CategoryLoadResult pantryCategories = {0};
  StockItem *newStockedFridge = NULL;
  StockItem *newStockedPantry = NULL;
  int newStockedFridgeCount = 0;
  int newStockedPantryCount = 0;

  JSON_Value *rootValue = json_parse_file(ingredientsPath);
  if (rootValue == NULL) {
    TraceLog(LOG_ERROR, "Could not parse ingredient file: %s", ingredientsPath);
    return false;
  }

  JSON_Object *rootObject = json_value_get_object(rootValue);
  JSON_Array *fridgeArray = (rootObject != NULL) ? json_object_get_array(rootObject, "fridge") : NULL;
  JSON_Array *pantryArray = (rootObject != NULL) ? json_object_get_array(rootObject, "pantry") : NULL;
  if (fridgeArray == NULL || pantryArray == NULL) {
    json_value_free(rootValue);
    TraceLog(LOG_ERROR, "Ingredient file must define both 'fridge' and 'pantry' arrays");
    return false;
  }

  if (!LoadCategories(fridgeArray, &fridgeCategories) ||
      !LoadCategories(pantryArray, &pantryCategories)) {
    json_value_free(rootValue);
    FreeCategories(fridgeCategories.categories, fridgeCategories.count);
    FreeCategories(pantryCategories.categories, pantryCategories.count);
    return false;
  }

  json_value_free(rootValue);

  if (!LoadStock(fridgeStockPath, fridgeCategories.categories, fridgeCategories.count, &newStockedFridge, &newStockedFridgeCount) ||
      !LoadStock(pantryStockPath, pantryCategories.categories, pantryCategories.count, &newStockedPantry, &newStockedPantryCount)) {
    FreeCategories(fridgeCategories.categories, fridgeCategories.count);
    FreeCategories(pantryCategories.categories, pantryCategories.count);
    free(newStockedFridge);
    free(newStockedPantry);
    return false;
  }

  UnloadItemData();

  allFridge = fridgeCategories.categories;
  allPantry = pantryCategories.categories;
  fridgeItemCountRuntime = fridgeCategories.count;
  pantryItemCountRuntime = pantryCategories.count;

  stockedFridge = newStockedFridge;
  stockedPantry = newStockedPantry;
  stockedFridgeCount = newStockedFridgeCount;
  stockedPantryCount = newStockedPantryCount;

  return true;
}

void UnloadItemData(void) {
  FreeCategories(allFridge, fridgeItemCountRuntime);
  FreeCategories(allPantry, pantryItemCountRuntime);
  allFridge = NULL;
  allPantry = NULL;
  fridgeItemCountRuntime = 0;
  pantryItemCountRuntime = 0;

  free(stockedFridge);
  free(stockedPantry);
  stockedFridge = NULL;
  stockedPantry = NULL;
  stockedFridgeCount = 0;
  stockedPantryCount = 0;
}
