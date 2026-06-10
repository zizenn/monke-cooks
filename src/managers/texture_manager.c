#include "core/textures.h" // includes raylib.h so we dont need to include it too
#include "external/raylib.h"

#if defined(_WIN32)
  #ifndef NOGDI
    #define NOGDI
  #endif
  #ifndef NOUSER
    #define NOUSER
  #endif
#endif

#include "external/parson.h"
#include "external/tinycthread.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// variables
AssetTexture* baseRegistry = NULL;
int totalBaseTextures = 0;

AssetTexture* levelRegistry = NULL;
int totalLevelTextures = 0;
int levelTexturesLoadedCount = 0;

static thrd_t levelLoadingThread;
static bool levelThreadFinished = true;

void LoadBaseTextureDatabase(const char* filename) {
    JSON_Value *rootValue = json_parse_file(filename);
    if (!rootValue) {
        printf("Error: Failed to read master base manifest: %s\n", filename);
        return;
    }
    JSON_Object *rootObject = json_value_get_object(rootValue);
    JSON_Array *texturesArray = json_object_get_array(rootObject, "textures");

    totalBaseTextures = (int)json_array_get_count(texturesArray);
    baseRegistry = malloc(sizeof(AssetTexture) * totalBaseTextures);

    for (int i = 0; i < totalBaseTextures; i++) {
        JSON_Object *texObj = json_array_get_object(texturesArray, i);
        const char* nameStr = json_object_get_string(texObj, "name");
        const char* pathStr = json_object_get_string(texObj, "filePath");

        baseRegistry[i].name = malloc(strlen(nameStr) + 1);
        strcpy(baseRegistry[i].name, nameStr);

        baseRegistry[i].texture = LoadTexture(pathStr); // running on main thread so no need for using LoadImage
        baseRegistry[i].filePath = NULL;
        baseRegistry[i].isLoaded = true;
    }
    json_value_free(rootValue);
    printf("texture system: %d base assets loaded.\n", totalBaseTextures);
}

int BackgroundLevelWorker(void* arg) {
    const char* path = (const char*)arg;

    JSON_Value *rootValue = json_parse_file(path);
    if (!rootValue) {
        printf("Thread Error: Could not read level manifest %s\n", path);
        levelThreadFinished = true;
        return -1;
    }
    JSON_Object *rootObject = json_value_get_object(rootValue);
    JSON_Array *texturesArray = json_object_get_array(rootObject, "textures");

    int count = (int)json_array_get_count(texturesArray);
    AssetTexture* registry = calloc(count, sizeof(AssetTexture));

    for (int i = 0; i < count; i++) {
        JSON_Object *texObj = json_array_get_object(texturesArray, i);
        const char* nameStr = json_object_get_string(texObj, "name");
        const char* pathStr = json_object_get_string(texObj, "filePath");

        registry[i].name = malloc(strlen(nameStr) + 1);
        strcpy(registry[i].name, nameStr);

        registry[i].filePath = malloc(strlen(pathStr) + 1);
        strcpy(registry[i].filePath, pathStr);

        registry[i].isLoaded = false;
        registry[i].texture = (Texture2D){ 0 };

        registry[i].rawImage = LoadImage(pathStr);
        registry[i].isLoaded = true;
    }

    levelRegistry = registry;
    totalLevelTextures = count;

    json_value_free(rootValue);
    levelThreadFinished = true;
    return 0;
}

void InitLevelTextureDatabase(const char* filename) {
    levelTexturesLoadedCount = 0;
    totalLevelTextures = 0;
    levelThreadFinished = false;
    levelRegistry = NULL;

    // background thread
    if (thrd_create(&levelLoadingThread, BackgroundLevelWorker, (void*)filename) != thrd_success) {
        printf("Error: Failed to create level loader thread context.\n");
        levelThreadFinished = true;
    }
}

void UpdateAsyncTextureLoading(void) {
    // skip checking entirely if nothing is pending
    if (levelThreadFinished && levelTexturesLoadedCount >= totalLevelTextures) {
        return;
    }

    for (int i = 0; i < totalLevelTextures; i++) {
        if (levelRegistry && levelRegistry[i].isLoaded && levelRegistry[i].texture.id == 0) {
            levelRegistry[i].texture = LoadTextureFromImage(levelRegistry[i].rawImage);
            UnloadImage(levelRegistry[i].rawImage);
            levelTexturesLoadedCount++;
        }
    }
}

bool IsTextureDatabaseReady(void) {
    if (!levelThreadFinished) return false;
    return levelTexturesLoadedCount >= totalLevelTextures;
}

Texture2D GetTextureByName(const char* name) {
    if (name == NULL) return (Texture2D){ 0 };

    // base textures
    for (int i = 0; i < totalBaseTextures; i++) {
        if (baseRegistry[i].name && strcmp(baseRegistry[i].name, name) == 0) {
            return baseRegistry[i].texture;
        }
    }

    // level textures
    for (int i = 0; i < totalLevelTextures; i++) {
        if (levelRegistry && levelRegistry[i].name && strcmp(levelRegistry[i].name, name) == 0) {
            return levelRegistry[i].texture;
        }
    }

    return (Texture2D){ 0 };
}

void UnloadLevelTextureDatabase(void) {
    if (!levelThreadFinished) {
        thrd_join(levelLoadingThread, NULL);
    }

    if (levelRegistry != NULL) {
        for (int i = 0; i < totalLevelTextures; i++) {
            free(levelRegistry[i].name);
            free(levelRegistry[i].filePath);
            if (levelRegistry[i].texture.id != 0) {
                UnloadTexture(levelRegistry[i].texture);
            }
        }
        free(levelRegistry);
        levelRegistry = NULL;
        totalLevelTextures = 0;
        levelTexturesLoadedCount = 0;
        printf("texture system: level textures freed\n");
    }
}

void UnloadBaseTextureDatabase(void) {
    if (baseRegistry != NULL) {
        for (int i = 0; i < totalBaseTextures; i++) {
            free(baseRegistry[i].name);
            if (baseRegistry[i].texture.id != 0) {
                UnloadTexture(baseRegistry[i].texture);
            }
        }
        free(baseRegistry);
        baseRegistry = NULL;
        totalBaseTextures = 0;
        printf("texture system: texture unloading successful");
    } else {
      printf("texture system: texture unloading failed");
    }
}
