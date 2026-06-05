#ifndef TEXTURE_HANDLER_H
#define TEXTURE_HANDLER_H

#include "external/raylib.h"

// structs
typedef struct {
    char* name;
    Texture2D texture;
} AssetTexture;

// variables
extern AssetTexture* textureRegistry;
extern int totalTextures;

// function prototypes
void LoadTextureDatabase(const char* filename);
void UnloadTextureDatabase(void);
Texture2D GetTextureByName(const char* name);

#endif
