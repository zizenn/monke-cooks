#include "game/texture_cache.h"
#include "external/raylib.h"
#include "stdbool.h"
#include "string.h"

typedef struct {
  bool isUsed;
  char filePath[260];
  Texture2D texture;
  int refCount;
} TextureCacheEntry;

static TextureCacheEntry textureCache[64] = {0};

Texture2D AcquireCachedTexture(const char *filePath) {
  if (filePath == NULL || filePath[0] == '\0') {
    return (Texture2D){0};
  }

  for (int i = 0; i < 64; i++) {
    if (!textureCache[i].isUsed) continue;
    if (strcmp(textureCache[i].filePath, filePath) == 0) {
      textureCache[i].refCount++;
      return textureCache[i].texture;
    }
  }

  for (int i = 0; i < 64; i++) {
    if (textureCache[i].isUsed) continue;

    Texture2D newTexture = LoadTexture(filePath);
    if (newTexture.id == 0) {
      return newTexture;
    }

    textureCache[i].isUsed = true;
    strncpy(textureCache[i].filePath, filePath, sizeof(textureCache[i].filePath) - 1);
    textureCache[i].filePath[sizeof(textureCache[i].filePath) - 1] = '\0';
    textureCache[i].texture = newTexture;
    textureCache[i].refCount = 1;
    return newTexture;
  }

  TraceLog(LOG_WARNING, "Texture cache full, loading unmanaged texture: %s", filePath);
  return LoadTexture(filePath);
}

void ReleaseTexture(Texture2D texture) {
  if (texture.id == 0) return;

  for (int i = 0; i < 64; i++) {
    if (!textureCache[i].isUsed) continue;
    if (textureCache[i].texture.id != texture.id) continue;

    textureCache[i].refCount--;
    if (textureCache[i].refCount <= 0) {
      UnloadTexture(textureCache[i].texture);
      textureCache[i] = (TextureCacheEntry){0};
    }
    return;
  }

  UnloadTexture(texture);
}

void ReleaseTextureArray(Texture2D *textures, int count) {
  if (textures == NULL || count <= 0) return;

  for (int i = 0; i < count; i++) {
    if (textures[i].id == 0) continue;

    bool alreadyReleased = false;
    for (int j = 0; j < i; j++) {
      if (textures[j].id == textures[i].id) {
        alreadyReleased = true;
        break;
      }
    }

    if (!alreadyReleased) {
      ReleaseTexture(textures[i]);
    }
  }

  for (int i = 0; i < count; i++) {
    textures[i] = (Texture2D){0};
  }
}

void FillTextureArray(Texture2D *textures, int count, Texture2D texture) {
  if (textures == NULL || count <= 0) return;

  for (int i = 0; i < count; i++) {
    textures[i] = texture;
  }
}
