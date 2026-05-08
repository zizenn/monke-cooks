#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#ifndef NOGDI
#define NOGDI
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "external/raylib.h"
#include "core/enums.h"

typedef enum {
  // Player textures
  PLAYER_UP,
  PLAYER_DOWN,
  PLAYER_LEFT,
  PLAYER_RIGHT,

  // Food - Eggs
  EGG_RAW,
  EGG_CRACKED,
  EGG_FRIED,

  // Food - Rice
  RICE_RAW,
  RICE_WASHED,
  RICE_COOKED,

  // Food - Shiitake
  SHIITAKE_RAW,
  SHIITAKE_SLICED,
  SHIITAKE_COOKED,

  // Food - Chickpea
  CHICKPEA_RAW,
  CHICKPEA_SOAKED,
  CHICKPEA_COOKED,

  // Food - Lentil
  LENTIL_RAW,
  LENTIL_RINSED,
  LENTIL_COOKED,

  // Food - Oyster Mushroom
  OYSTER_MUSHROOM_RAW,
  OYSTER_MUSHROOM_SLICED,
  OYSTER_MUSHROOM_COOKED,

  // Food - Mozzarella
  MOZZARELLA_FRESH,
  MOZZARELLA_MELTED,

  // Food - Parmigiano
  PARMIGIANO_FRESH,
  PARMIGIANO_MELTED,

  // Food - Cheddar
  CHEDDAR_FRESH,
  CHEDDAR_MELTED,

  // Food - Pepper
  PEPPER_RAW,
  PEPPER_GROUND,

  // Food - Cumin
  CUMIN_RAW,
  CUMIN_ROASTED,

  // Pantry - Turmeric
  TURMERIC_RAW,
  TURMERIC_GROUND,

  // Pantry - Vanilla
  VANILLA_RAW,
  VANILLA_PROCESSED,

  // Pantry - Star Anise
  STAR_ANISE_WHOLE,
  STAR_ANISE_GROUND,

  // Pantry - Cloves
  CLOVES_WHOLE,
  CLOVES_GROUND,

  // Pantry - Nutmeg
  NUTMEG_WHOLE,
  NUTMEG_GROUND,

  // Pantry - Wheat
  WHEAT_RAW,
  WHEAT_MILLED,

  // Pantry - Barley
  BARLEY_RAW,
  BARLEY_COOKED,

  // Pantry - Nori
  NORI_DRIED,
  NORI_TOASTED,

  // Pantry - Wakame
  WAKAME_DRIED,
  WAKAME_SOAKED,

  // Pantry - Milk
  MILK_FRESH,
  MILK_HEATED,

  TEXTURE_COUNT
} TextureID;

typedef struct {
  const char *name;
  const char *filePath;
  Texture2D texture;
} TextureAsset;

// Texture mapping entry: maps (categoryId, variantId, origin) -> TextureID
typedef struct {
  int categoryId;
  int variantId;
  ITEM_ORIGIN origin;
  TextureID textureId;
} TextureMapEntry;

// Dynamic texture map
typedef struct {
  TextureMapEntry *entries;
  int count;
  int capacity;
} TextureMap;

extern TextureAsset allTextures[TEXTURE_COUNT];
extern TextureMap textureMap;

// Texture management API
void ProcessTextureLoadingOnMainThread(void);
void InitTextureMap(void);
void AddTextureMapping(int categoryId, int variantId, ITEM_ORIGIN origin, TextureID textureId);
void FreeTextureMap(void);
void UnloadAllTextures(void);
Texture2D GetTexture(TextureID id);
Texture2D GetHeldItemTexture();

#endif
