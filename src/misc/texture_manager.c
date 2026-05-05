#ifndef NOGDI
#define NOGDI
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "game/texture_manager.h"
#include "game/items.h"
#include "game/game.h"
#include "external/raylib.h"
#include "string.h"

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
void LoadAllImagesAsync(void) {
  for (int i = 0; i < TEXTURE_COUNT; i++) {
    imageCache[i].image = LoadImage(allTextures[i].filePath);
    imageCache[i].loaded = 1;
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

Texture2D GetHeldItemTexture(int categoryId, int variantId, int itemFrom) {
  if (itemFrom == FROM_FRIDGE) {
    // Map fridge items (0-10)
    if (categoryId == 0) { // EGG
      if (variantId == 0) return GetTexture(EGG_RAW);
      if (variantId == 1) return GetTexture(EGG_CRACKED);
      if (variantId == 2) return GetTexture(EGG_FRIED);
    } else if (categoryId == 1) { // RICE
      if (variantId == 0) return GetTexture(RICE_RAW);
      if (variantId == 1) return GetTexture(RICE_WASHED);
      if (variantId == 2) return GetTexture(RICE_COOKED);
    } else if (categoryId == 2) { // SHIITAKE
      if (variantId == 0) return GetTexture(SHIITAKE_RAW);
      if (variantId == 1) return GetTexture(SHIITAKE_SLICED);
      if (variantId == 2) return GetTexture(SHIITAKE_COOKED);
    } else if (categoryId == 3) { // CHICKPEA
      if (variantId == 0) return GetTexture(CHICKPEA_RAW);
      if (variantId == 1) return GetTexture(CHICKPEA_SOAKED);
      if (variantId == 2) return GetTexture(CHICKPEA_COOKED);
    } else if (categoryId == 4) { // LENTIL
      if (variantId == 0) return GetTexture(LENTIL_RAW);
      if (variantId == 1) return GetTexture(LENTIL_RINSED);
      if (variantId == 2) return GetTexture(LENTIL_COOKED);
    } else if (categoryId == 5) { // OYSTER_MUSHROOM
      if (variantId == 0) return GetTexture(OYSTER_MUSHROOM_RAW);
      if (variantId == 1) return GetTexture(OYSTER_MUSHROOM_SLICED);
      if (variantId == 2) return GetTexture(OYSTER_MUSHROOM_COOKED);
    } else if (categoryId == 6) { // MOZZARELLA
      if (variantId == 0) return GetTexture(MOZZARELLA_FRESH);
      if (variantId == 1) return GetTexture(MOZZARELLA_MELTED);
    } else if (categoryId == 7) { // PARMIGIANO
      if (variantId == 0) return GetTexture(PARMIGIANO_FRESH);
      if (variantId == 1) return GetTexture(PARMIGIANO_MELTED);
    } else if (categoryId == 8) { // CHEDDAR
      if (variantId == 0) return GetTexture(CHEDDAR_FRESH);
      if (variantId == 1) return GetTexture(CHEDDAR_MELTED);
    } else if (categoryId == 9) { // PEPPER
      if (variantId == 0) return GetTexture(PEPPER_RAW);
      if (variantId == 1) return GetTexture(PEPPER_GROUND);
    } else if (categoryId == 10) { // CUMIN
      if (variantId == 0) return GetTexture(CUMIN_RAW);
      if (variantId == 1) return GetTexture(CUMIN_ROASTED);
    }
  } else if (itemFrom == FROM_PANTRY) {
    // Map pantry items (0-9)
    if (categoryId == 0) { // TURMERIC
      if (variantId == 0) return GetTexture(TURMERIC_RAW);
      if (variantId == 1) return GetTexture(TURMERIC_GROUND);
    } else if (categoryId == 1) { // VANILLA
      if (variantId == 0) return GetTexture(VANILLA_RAW);
      if (variantId == 1) return GetTexture(VANILLA_PROCESSED);
    } else if (categoryId == 2) { // STAR_ANISE
      if (variantId == 0) return GetTexture(STAR_ANISE_WHOLE);
      if (variantId == 1) return GetTexture(STAR_ANISE_GROUND);
    } else if (categoryId == 3) { // CLOVES
      if (variantId == 0) return GetTexture(CLOVES_WHOLE);
      if (variantId == 1) return GetTexture(CLOVES_GROUND);
    } else if (categoryId == 4) { // NUTMEG
      if (variantId == 0) return GetTexture(NUTMEG_WHOLE);
      if (variantId == 1) return GetTexture(NUTMEG_GROUND);
    } else if (categoryId == 5) { // WHEAT
      if (variantId == 0) return GetTexture(WHEAT_RAW);
      if (variantId == 1) return GetTexture(WHEAT_MILLED);
    } else if (categoryId == 6) { // BARLEY
      if (variantId == 0) return GetTexture(BARLEY_RAW);
      if (variantId == 1) return GetTexture(BARLEY_COOKED);
    } else if (categoryId == 7) { // NORI
      if (variantId == 0) return GetTexture(NORI_DRIED);
      if (variantId == 1) return GetTexture(NORI_TOASTED);
    } else if (categoryId == 8) { // WAKAME
      if (variantId == 0) return GetTexture(WAKAME_DRIED);
      if (variantId == 1) return GetTexture(WAKAME_SOAKED);
    } else if (categoryId == 9) { // MILK
      if (variantId == 0) return GetTexture(MILK_FRESH);
      if (variantId == 1) return GetTexture(MILK_HEATED);
    }
  }

  return (Texture2D){0};
}
