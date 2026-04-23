#ifndef TEXTURE_CACHE_H
#define TEXTURE_CACHE_H

#include "external/raylib.h"

Texture2D AcquireCachedTexture(const char *filePath);
void ReleaseTexture(Texture2D texture);
void ReleaseTextureArray(Texture2D *textures, int count);
void FillTextureArray(Texture2D *textures, int count, Texture2D texture);

#endif
