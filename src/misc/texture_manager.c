#include "game/texture_manager.h"
#include "core/enums.h"
#include "external/raylib.h"
#include "external/parson.h"
#include "stdlib.h"
#include "string.h"
#include "game/globals.h"
#include "game/items.h"

TextureMap textureMap = {0};

TextureAsset allTextures[TEXTURE_COUNT] = {0};

// Image cache used by thread_manager for async loading
typedef struct {
  Image image;
  bool loaded;
} ImageCache;

static ImageCache imageCache[TEXTURE_COUNT] = {0};
static const char *textureIdNames[TEXTURE_COUNT] = {
  "PLAYER_UP",
  "PLAYER_DOWN",
  "PLAYER_LEFT",
  "PLAYER_RIGHT",
  "EGG_RAW",
  "EGG_CRACKED",
  "EGG_FRIED",
  "RICE_RAW",
  "RICE_WASHED",
  "RICE_COOKED",
  "SHIITAKE_RAW",
  "SHIITAKE_SLICED",
  "SHIITAKE_COOKED",
  "CHICKPEA_RAW",
  "CHICKPEA_SOAKED",
  "CHICKPEA_COOKED",
  "LENTIL_RAW",
  "LENTIL_RINSED",
  "LENTIL_COOKED",
  "OYSTER_MUSHROOM_RAW",
  "OYSTER_MUSHROOM_SLICED",
  "OYSTER_MUSHROOM_COOKED",
  "MOZZARELLA_FRESH",
  "MOZZARELLA_MELTED",
  "PARMIGIANO_FRESH",
  "PARMIGIANO_MELTED",
  "CHEDDAR_FRESH",
  "CHEDDAR_MELTED",
  "PEPPER_RAW",
  "PEPPER_GROUND",
  "CUMIN_RAW",
  "CUMIN_ROASTED",
  "TURMERIC_RAW",
  "TURMERIC_GROUND",
  "VANILLA_RAW",
  "VANILLA_PROCESSED",
  "STAR_ANISE_WHOLE",
  "STAR_ANISE_GROUND",
  "CLOVES_WHOLE",
  "CLOVES_GROUND",
  "NUTMEG_WHOLE",
  "NUTMEG_GROUND",
  "WHEAT_RAW",
  "WHEAT_MILLED",
  "BARLEY_RAW",
  "BARLEY_COOKED",
  "NORI_DRIED",
  "NORI_TOASTED",
  "WAKAME_DRIED",
  "WAKAME_SOAKED",
  "MILK_FRESH",
  "MILK_HEATED"
};

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

static void FreeTextureMetadata(void) {
  for (int i = 0; i < TEXTURE_COUNT; i++) {
    if (allTextures[i].name != NULL) {
      free((void *)allTextures[i].name);
      allTextures[i].name = NULL;
    }
    if (allTextures[i].filePath != NULL) {
      free((void *)allTextures[i].filePath);
      allTextures[i].filePath = NULL;
    }
  }
}

static int FindTextureIdIndexByName(const char *name) {
  if (name == NULL) {
    return -1;
  }

  for (int i = 0; i < TEXTURE_COUNT; i++) {
    if (strcmp(textureIdNames[i], name) == 0) {
      return i;
    }
  }

  return -1;
}

bool LoadTextureManifest(const char *path) {
  JSON_Value *rootValue = json_parse_file(path);
  if (rootValue == NULL) {
    TraceLog(LOG_ERROR, "Could not parse texture manifest: %s", path);
    return false;
  }

  JSON_Object *rootObject = json_value_get_object(rootValue);
  JSON_Array *texturesArray = (rootObject != NULL) ? json_object_get_array(rootObject, "textures") : NULL;
  if (texturesArray == NULL) {
    json_value_free(rootValue);
    TraceLog(LOG_ERROR, "Texture manifest missing 'textures' array: %s", path);
    return false;
  }

  size_t textureCount = json_array_get_count(texturesArray);
  if ((int)textureCount != TEXTURE_COUNT) {
    json_value_free(rootValue);
    TraceLog(LOG_ERROR, "Texture manifest count mismatch. expected=%d got=%d", TEXTURE_COUNT, (int)textureCount);
    return false;
  }

  bool seen[TEXTURE_COUNT] = {0};
  FreeTextureMetadata();
  for (size_t i = 0; i < textureCount; i++) {
    JSON_Object *textureObj = json_array_get_object(texturesArray, i);
    if (textureObj == NULL) {
      json_value_free(rootValue);
      FreeTextureMetadata();
      return false;
    }

    const char *name = json_object_get_string(textureObj, "name");
    const char *filePath = json_object_get_string(textureObj, "filePath");
    if (name == NULL || filePath == NULL) {
      json_value_free(rootValue);
      FreeTextureMetadata();
      TraceLog(LOG_ERROR, "Invalid texture manifest entry at index %d", (int)i);
      return false;
    }

    int textureIndex = FindTextureIdIndexByName(name);
    if (textureIndex < 0) {
      json_value_free(rootValue);
      FreeTextureMetadata();
      TraceLog(LOG_ERROR, "Unknown texture name in manifest: %s", name);
      return false;
    }

    if (seen[textureIndex]) {
      json_value_free(rootValue);
      FreeTextureMetadata();
      TraceLog(LOG_ERROR, "Duplicate texture name in manifest: %s", name);
      return false;
    }

    allTextures[textureIndex].name = CopyString(name);
    allTextures[textureIndex].filePath = CopyString(filePath);
    allTextures[textureIndex].texture = (Texture2D){0};
    if (allTextures[textureIndex].name == NULL || allTextures[textureIndex].filePath == NULL) {
      json_value_free(rootValue);
      FreeTextureMetadata();
      return false;
    }
    seen[textureIndex] = true;
  }

  for (int i = 0; i < TEXTURE_COUNT; i++) {
    if (!seen[i]) {
      json_value_free(rootValue);
      FreeTextureMetadata();
      TraceLog(LOG_ERROR, "Missing texture entry in manifest: %s", textureIdNames[i]);
      return false;
    }
  }

  json_value_free(rootValue);
  return true;
}

// loads images without putting into gpu (so its opengl safe :) )
void LoadAllTexturesAsync(void) {
  for (int i = 0; i < TEXTURE_COUNT; i++) {
    if (allTextures[i].filePath == NULL) {
      imageCache[i].loaded = false;
      TraceLog(LOG_ERROR, "Texture manifest was not loaded before async texture load");
      continue;
    }
    imageCache[i].image = LoadImage(allTextures[i].filePath);
    imageCache[i].loaded = true;
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
      imageCache[i].loaded = false;
    }
  }
}

void UnloadAllTextures(void) {
  for (int i = 0; i < TEXTURE_COUNT; i++) {
    if (allTextures[i].texture.id != 0) {
      UnloadTexture(allTextures[i].texture);
      allTextures[i].texture = (Texture2D){0};
    }
    imageCache[i].image = (Image){0};
    imageCache[i].loaded = false;
  }
  FreeTextureMetadata();
}

Texture2D GetTexture(TextureID id) {
  if (id >= 0 && id < TEXTURE_COUNT) {
    return allTextures[id].texture;
  }
  return (Texture2D){0};
}

// ===== DYNAMIC TEXTURE MAP FUNCTIONS =====

void InitTextureMap(void) {
  int initialCapacity = 1;
  for (int i = 0; i < fridgeItemCountRuntime; i++) {
    initialCapacity += allFridge[i].variantCount;
  }
  for (int i = 0; i < pantryItemCountRuntime; i++) {
    initialCapacity += allPantry[i].variantCount;
  }

  textureMap.entries = (TextureMapEntry *)calloc(initialCapacity, sizeof(TextureMapEntry));
  textureMap.capacity = initialCapacity;
  textureMap.count = 0;

  for (int i = 0; i < fridgeItemCountRuntime; i++) {
    for (int j = 0; j < allFridge[i].variantCount; j++) {
      AddTextureMapping(i, j, FROM_FRIDGE, (TextureID)allFridge[i].variants[j].textureId);
    }
  }

  for (int i = 0; i < pantryItemCountRuntime; i++) {
    for (int j = 0; j < allPantry[i].variantCount; j++) {
      AddTextureMapping(i, j, FROM_PANTRY, (TextureID)allPantry[i].variants[j].textureId);
    }
  }
}

void AddTextureMapping(int categoryId, int variantId, ITEM_ORIGIN origin, TextureID textureId) {
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

Texture2D GetHeldItemTexture() {
  for (int i = 0; i < textureMap.count; i++) {
    if (textureMap.entries[i].categoryId == holding.categoryId &&
        textureMap.entries[i].variantId == holding.variantId &&
        textureMap.entries[i].origin == holding.origin) {
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
