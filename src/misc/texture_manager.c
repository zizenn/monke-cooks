#include "game/texture_manager.h"
#include "external/raylib.h"
#include "stdlib.h"

TextureMap textureMap = {0};

TextureAsset allTextures[TEXTURE_COUNT] = {
  // Player textures
  { "PLAYER_UP", "assets/monkey/imgs/up.png", {0} },
  { "PLAYER_DOWN", "assets/monkey/imgs/down.png", {0} },
  { "PLAYER_LEFT", "assets/monkey/imgs/left.png", {0} },
  { "PLAYER_RIGHT", "assets/monkey/imgs/right.png", {0} },

  // Food - Eggs
  { "EGG_RAW", "assets/food/ingredients/eggs/egg_orange.png", {0} },
  { "EGG_CRACKED", "assets/food/ingredients/eggs/egg_orange_cracked.png", {0} },
  { "EGG_FRIED", "assets/food/dishes/38_friedegg.png", {0} },

  // Food - Rice
  { "RICE_RAW", "assets/food/ingredients/grains/rice.png", {0} },
  { "RICE_WASHED", "assets/food/ingredients/grains/rice.png", {0} },
  { "RICE_COOKED", "assets/food/ingredients/grains/rice.png", {0} },

  // Food - Shiitake
  { "SHIITAKE_RAW", "assets/food/ingredients/mushrooms/shiitake.png", {0} },
  { "SHIITAKE_SLICED", "assets/food/ingredients/mushrooms/hen_of_the_woods_sliced.png", {0} },
  { "SHIITAKE_COOKED", "assets/food/ingredients/mushrooms/shiitake.png", {0} },

  // Food - Chickpea
  { "CHICKPEA_RAW", "assets/food/ingredients/beans/chickpea.png", {0} },
  { "CHICKPEA_SOAKED", "assets/food/ingredients/beans/chickpea.png", {0} },
  { "CHICKPEA_COOKED", "assets/food/ingredients/beans/chickpea.png", {0} },

  // Food - Lentil
  { "LENTIL_RAW", "assets/food/ingredients/beans/lentil.png", {0} },
  { "LENTIL_RINSED", "assets/food/ingredients/beans/lentil.png", {0} },
  { "LENTIL_COOKED", "assets/food/ingredients/beans/lentil.png", {0} },

  // Food - Oyster Mushroom
  { "OYSTER_MUSHROOM_RAW", "assets/food/ingredients/mushrooms/oyster_mushroom.png", {0} },
  { "OYSTER_MUSHROOM_SLICED", "assets/food/ingredients/mushrooms/oyster_mushroom.png", {0} },
  { "OYSTER_MUSHROOM_COOKED", "assets/food/ingredients/mushrooms/oyster_mushroom.png", {0} },

  // Food - Mozzarella
  { "MOZZARELLA_FRESH", "assets/food/ingredients/cheese/mozzarella.png", {0} },
  { "MOZZARELLA_MELTED", "assets/food/ingredients/cheese/mozzarella.png", {0} },

  // Food - Parmigiano
  { "PARMIGIANO_FRESH", "assets/food/ingredients/cheese/parmigiano_reggiano.png", {0} },
  { "PARMIGIANO_MELTED", "assets/food/ingredients/cheese/parmigiano_reggiano.png", {0} },

  // Food - Cheddar
  { "CHEDDAR_FRESH", "assets/food/ingredients/cheese/cheddar.png", {0} },
  { "CHEDDAR_MELTED", "assets/food/ingredients/cheese/cheddar.png", {0} },

  // Food - Pepper
  { "PEPPER_RAW", "assets/food/ingredients/spices/pepper.png", {0} },
  { "PEPPER_GROUND", "assets/food/ingredients/spices/pepper.png", {0} },

  // Food - Cumin
  { "CUMIN_RAW", "assets/food/ingredients/spices/cumin.png", {0} },
  { "CUMIN_ROASTED", "assets/food/ingredients/spices/cumin.png", {0} },

  // Pantry - Turmeric
  { "TURMERIC_RAW", "assets/food/ingredients/spices/turmeric.png", {0} },
  { "TURMERIC_GROUND", "assets/food/ingredients/spices/turmeric.png", {0} },

  // Pantry - Vanilla
  { "VANILLA_RAW", "assets/food/ingredients/spices/vanilla.png", {0} },
  { "VANILLA_PROCESSED", "assets/food/ingredients/spices/vanilla.png", {0} },

  // Pantry - Star Anise
  { "STAR_ANISE_WHOLE", "assets/food/ingredients/spices/star_anise.png", {0} },
  { "STAR_ANISE_GROUND", "assets/food/ingredients/spices/star_anise.png", {0} },

  // Pantry - Cloves
  { "CLOVES_WHOLE", "assets/food/ingredients/spices/cloves.png", {0} },
  { "CLOVES_GROUND", "assets/food/ingredients/spices/cloves.png", {0} },

  // Pantry - Nutmeg
  { "NUTMEG_WHOLE", "assets/food/ingredients/spices/nutmeg.png", {0} },
  { "NUTMEG_GROUND", "assets/food/ingredients/spices/nutmeg.png", {0} },

  // Pantry - Wheat
  { "WHEAT_RAW", "assets/food/ingredients/grains/wheat.png", {0} },
  { "WHEAT_MILLED", "assets/food/ingredients/grains/wheat.png", {0} },

  // Pantry - Barley
  { "BARLEY_RAW", "assets/food/ingredients/grains/barley.png", {0} },
  { "BARLEY_COOKED", "assets/food/ingredients/grains/barley.png", {0} },

  // Pantry - Nori
  { "NORI_DRIED", "assets/food/ingredients/seaweeds/nori_toasted.png", {0} },
  { "NORI_TOASTED", "assets/food/ingredients/seaweeds/nori_toasted.png", {0} },

  // Pantry - Wakame
  { "WAKAME_DRIED", "assets/food/ingredients/seaweeds/wakame.png", {0} },
  { "WAKAME_SOAKED", "assets/food/ingredients/seaweeds/wakame.png", {0} },

  // Pantry - Milk
  { "MILK_FRESH", "assets/food/ingredients/yogurt_and_milk/milk_bottled.png", {0} },
  { "MILK_HEATED", "assets/food/ingredients/yogurt_and_milk/milk_bottled.png", {0} },
};

// Image cache used by thread_manager for async loading
typedef struct {
  Image image;
  int loaded;
} ImageCache;

static ImageCache imageCache[TEXTURE_COUNT] = {0};

// Called by thread_manager to load images from disk
void LoadAllTexturesAsync(void) {
  for (int i = 0; i < TEXTURE_COUNT; i++) {
    imageCache[i].image = LoadImage(allTextures[i].filePath);
    imageCache[i].loaded = i;
  }
}

// Called by main thread to convert loaded images to textures
void ProcessTextureLoadingOnMainThread(void) {
  for (int i = 0; i < TEXTURE_COUNT; i++) {
    if (imageCache[i].loaded && allTextures[i].texture.id == 0) {
      allTextures[i].texture = LoadTextureFromImage(imageCache[i].image);
      if (allTextures[i].texture.id == 0) {
        TraceLog(LOG_WARNING, "Failed to load texture: %s (%s)", allTextures[i].name, allTextures[i].filePath);
      }
      UnloadImage(imageCache[i].image);
      imageCache[i].image = (Image){0};
    }
  }
}

void UnloadAllTextures(void) {
  for (int i = 0; i < TEXTURE_COUNT; i++) {
    if (allTextures[i].texture.id != 0) {
      UnloadTexture(allTextures[i].texture);
      allTextures[i].texture = (Texture2D){0};
    }
  }
}

Texture2D GetTexture(TextureID id) {
  if (id >= 0 && id < TEXTURE_COUNT) {
    return allTextures[id].texture;
  }
  return (Texture2D){0};
}

// ===== DYNAMIC TEXTURE MAP FUNCTIONS =====

void InitTextureMap(void) {
  int initialCapacity = 100;
  textureMap.entries = (TextureMapEntry *)calloc(initialCapacity, sizeof(TextureMapEntry));
  textureMap.capacity = initialCapacity;
  textureMap.count = 0;

  // Populate fridge items (FROM_FRIDGE)
  // 0: EGG (3 variants)
  AddTextureMapping(0, 0, FROM_FRIDGE, EGG_RAW);
  AddTextureMapping(0, 1, FROM_FRIDGE, EGG_CRACKED);
  AddTextureMapping(0, 2, FROM_FRIDGE, EGG_FRIED);
  // 1: RICE (3 variants)
  AddTextureMapping(1, 0, FROM_FRIDGE, RICE_RAW);
  AddTextureMapping(1, 1, FROM_FRIDGE, RICE_WASHED);
  AddTextureMapping(1, 2, FROM_FRIDGE, RICE_COOKED);
  // 2: SHIITAKE (3 variants)
  AddTextureMapping(2, 0, FROM_FRIDGE, SHIITAKE_RAW);
  AddTextureMapping(2, 1, FROM_FRIDGE, SHIITAKE_SLICED);
  AddTextureMapping(2, 2, FROM_FRIDGE, SHIITAKE_COOKED);
  // 3: CHICKPEA (3 variants)
  AddTextureMapping(3, 0, FROM_FRIDGE, CHICKPEA_RAW);
  AddTextureMapping(3, 1, FROM_FRIDGE, CHICKPEA_SOAKED);
  AddTextureMapping(3, 2, FROM_FRIDGE, CHICKPEA_COOKED);
  // 4: LENTIL (3 variants)
  AddTextureMapping(4, 0, FROM_FRIDGE, LENTIL_RAW);
  AddTextureMapping(4, 1, FROM_FRIDGE, LENTIL_RINSED);
  AddTextureMapping(4, 2, FROM_FRIDGE, LENTIL_COOKED);
  // 5: OYSTER_MUSHROOM (3 variants)
  AddTextureMapping(5, 0, FROM_FRIDGE, OYSTER_MUSHROOM_RAW);
  AddTextureMapping(5, 1, FROM_FRIDGE, OYSTER_MUSHROOM_SLICED);
  AddTextureMapping(5, 2, FROM_FRIDGE, OYSTER_MUSHROOM_COOKED);
  // 6: MOZZARELLA (2 variants)
  AddTextureMapping(6, 0, FROM_FRIDGE, MOZZARELLA_FRESH);
  AddTextureMapping(6, 1, FROM_FRIDGE, MOZZARELLA_MELTED);
  // 7: PARMIGIANO (2 variants)
  AddTextureMapping(7, 0, FROM_FRIDGE, PARMIGIANO_FRESH);
  AddTextureMapping(7, 1, FROM_FRIDGE, PARMIGIANO_MELTED);
  // 8: CHEDDAR (2 variants)
  AddTextureMapping(8, 0, FROM_FRIDGE, CHEDDAR_FRESH);
  AddTextureMapping(8, 1, FROM_FRIDGE, CHEDDAR_MELTED);
  // 9: PEPPER (2 variants)
  AddTextureMapping(9, 0, FROM_FRIDGE, PEPPER_RAW);
  AddTextureMapping(9, 1, FROM_FRIDGE, PEPPER_GROUND);
  // 10: CUMIN (2 variants)
  AddTextureMapping(10, 0, FROM_FRIDGE, CUMIN_RAW);
  AddTextureMapping(10, 1, FROM_FRIDGE, CUMIN_ROASTED);

  // Populate pantry items (FROM_PANTRY) - KEY: categoryId=9 maps to MILK here, not PEPPER
  // 0: TURMERIC (2 variants)
  AddTextureMapping(0, 0, FROM_PANTRY, TURMERIC_RAW);
  AddTextureMapping(0, 1, FROM_PANTRY, TURMERIC_GROUND);
  // 1: VANILLA (2 variants)
  AddTextureMapping(1, 0, FROM_PANTRY, VANILLA_RAW);
  AddTextureMapping(1, 1, FROM_PANTRY, VANILLA_PROCESSED);
  // 2: STAR_ANISE (2 variants)
  AddTextureMapping(2, 0, FROM_PANTRY, STAR_ANISE_WHOLE);
  AddTextureMapping(2, 1, FROM_PANTRY, STAR_ANISE_GROUND);
  // 3: CLOVES (2 variants)
  AddTextureMapping(3, 0, FROM_PANTRY, CLOVES_WHOLE);
  AddTextureMapping(3, 1, FROM_PANTRY, CLOVES_GROUND);
  // 4: NUTMEG (2 variants)
  AddTextureMapping(4, 0, FROM_PANTRY, NUTMEG_WHOLE);
  AddTextureMapping(4, 1, FROM_PANTRY, NUTMEG_GROUND);
  // 5: WHEAT (2 variants)
  AddTextureMapping(5, 0, FROM_PANTRY, WHEAT_RAW);
  AddTextureMapping(5, 1, FROM_PANTRY, WHEAT_MILLED);
  // 6: BARLEY (2 variants)
  AddTextureMapping(6, 0, FROM_PANTRY, BARLEY_RAW);
  AddTextureMapping(6, 1, FROM_PANTRY, BARLEY_COOKED);
  // 7: NORI (2 variants)
  AddTextureMapping(7, 0, FROM_PANTRY, NORI_DRIED);
  AddTextureMapping(7, 1, FROM_PANTRY, NORI_TOASTED);
  // 8: WAKAME (2 variants)
  AddTextureMapping(8, 0, FROM_PANTRY, WAKAME_DRIED);
  AddTextureMapping(8, 1, FROM_PANTRY, WAKAME_SOAKED);
  // 9: MILK (2 variants) - THIS IS THE FIX! Pantry milk stays milk, doesn't become pepper
  AddTextureMapping(9, 0, FROM_PANTRY, MILK_FRESH);
  AddTextureMapping(9, 1, FROM_PANTRY, MILK_HEATED);
}

void AddTextureMapping(int categoryId, int variantId, ItemOrigin origin, TextureID textureId) {
  // Resize if needed
  if (textureMap.count >= textureMap.capacity) {
    int newCapacity = textureMap.capacity * 2;
    textureMap.entries = (TextureMapEntry *)realloc(textureMap.entries, newCapacity * sizeof(TextureMapEntry));
    textureMap.capacity = newCapacity;
  }

  // Add entry
  textureMap.entries[textureMap.count].categoryId = categoryId;
  textureMap.entries[textureMap.count].variantId = variantId;
  textureMap.entries[textureMap.count].origin = origin;
  textureMap.entries[textureMap.count].textureId = textureId;
  textureMap.count++;
}

Texture2D GetHeldItemTexture(int categoryId, int variantId, int itemFrom) {
  for (int i = 0; i < textureMap.count; i++) {
    if (textureMap.entries[i].categoryId == categoryId &&
        textureMap.entries[i].variantId == variantId &&
        textureMap.entries[i].origin == itemFrom) {
      return GetTexture(textureMap.entries[i].textureId);
    }
  }
  return (Texture2D){0};
}

void FreeTextureMap(void) {
  if (textureMap.entries != NULL) {
    free(textureMap.entries);
    textureMap.entries = NULL;
    textureMap.count = 0;
    textureMap.capacity = 0;
  }
}
