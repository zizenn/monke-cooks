#include "core/textures.h"
#include "external/parson.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Allocate global variables
AssetTexture* textureRegistry = NULL;
int totalTextures = 0;

void LoadTextureDatabase(const char* filename) {
    JSON_Value *rootValue = json_parse_file(filename);
    if (rootValue == NULL) {
        printf("Error: Failed to parse texture manifest file: %s\n", filename);
        return;
    }

    JSON_Object *rootObject = json_value_get_object(rootValue);
    JSON_Array *texturesArray = json_object_get_array(rootObject, "textures");

    totalTextures = (int)json_array_get_count(texturesArray);
    textureRegistry = malloc(sizeof(AssetTexture) * totalTextures);

    for (int i = 0; i < totalTextures; i++) {
        JSON_Object *texObj = json_array_get_object(texturesArray, i);

        const char* nameStr = json_object_get_string(texObj, "name");
        const char* pathStr = json_object_get_string(texObj, "filePath");

        size_t nameLength = strlen(nameStr) + 1;
        textureRegistry[i].name = malloc(nameLength);
        if (textureRegistry[i].name != NULL) {
            strcpy(textureRegistry[i].name, nameStr);
        }

        textureRegistry[i].texture = LoadTexture(pathStr);

        printf("VRAM Loaded: [%s] from %s\n", nameStr, pathStr);
    }

    json_value_free(rootValue);
    printf("Texture Handler: Successfully loaded %d assets!\n", totalTextures);
}

Texture2D GetTextureByName(const char* name) {
    for (int i = 0; i < totalTextures; i++) {
        if (strcmp(textureRegistry[i].name, name) == 0) {
            return textureRegistry[i].texture;
        }
    }

    printf("Warning: Texture lookup failed for name: %s\n", name);
    return (Texture2D){ 0 };
}

void UnloadTextureDatabase(void) {
    if (textureRegistry != NULL) {
        for (int i = 0; i < totalTextures; i++) {
            if (textureRegistry[i].name != NULL) {
                free(textureRegistry[i].name);
            }
            UnloadTexture(textureRegistry[i].texture);
        }
        free(textureRegistry);
        textureRegistry = NULL;
        totalTextures = 0;
        printf("Texture database fully cleared from VRAM.\n");
    }
}
