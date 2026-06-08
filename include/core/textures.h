#ifndef TEXTURE_HANDLER_H
#define TEXTURE_HANDLER_H

#include "external/raylib.h"

// structs
typedef struct {
    char* name;
    char* filePath;
    Image rawImage;
    Texture2D texture;
    bool isLoaded;
} AssetTexture;

// variables
extern AssetTexture* baseRegistry;
extern int totalBaseTextures;
extern AssetTexture* levelRegistry;
extern int totalLevelTextures;
extern int levelTexturesLoadedCount;

// function prototypes
void LoadBaseTextureDatabase(const char* filename);
void InitLevelTextureDatabase(const char* filename);
void UpdateAsyncTextureLoading(void);
bool IsTextureDatabaseReady(void);
Texture2D GetTextureByName(const char* name);
void UnloadLevelTextureDatabase(void);
void UnloadBaseTextureDatabase(void);

#endif
