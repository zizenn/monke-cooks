#include "external/parson.h"
#include "core/map.h"
#include "core/config.h"
#include "external/raylib.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// variables
TileType* tileDatabase = NULL;
int tileDbSize = 0;
MapItem* levelTiles = NULL;
int totalTiles = 0;

Color ParseRaylibColor(JSON_Object* tile_object) {
  Color parsedColor = WHITE; // Default fallback

  JSON_Array* colorArray = json_object_get_array(tile_object, "fallbackColor");

  // Ensure the array exists and has exactly 4 elements (R, G, B, A)
  if (colorArray != NULL && json_array_get_count(colorArray) == 4) {
    parsedColor.r = (unsigned char)json_array_get_number(colorArray, 0);
    parsedColor.g = (unsigned char)json_array_get_number(colorArray, 1);
    parsedColor.b = (unsigned char)json_array_get_number(colorArray, 2);
    parsedColor.a = (unsigned char)json_array_get_number(colorArray, 3);
  } else {
    printf("Warning: Invalid or missing 'fallbackColor' array. Using default.\n");
  }

  return parsedColor;
}

// loading the tile database from JSON file
void LoadTileDatabase(const char* filename) {
    // parse the file
    JSON_Value *rootValue = json_parse_file(filename);
    if (rootValue == NULL) {
        printf("error: failed to parse tile registry file: %s\n", filename);
        return;
    }

    // get the root object and the tile_types array
    JSON_Object *rootObject = json_value_get_object(rootValue);
    JSON_Array *typesArray  = json_object_get_array(rootObject, "tile_types");

    // size c array to match the JSON array count
    tileDbSize = (int)json_array_get_count(typesArray);
    tileDatabase = malloc(sizeof(TileType) * tileDbSize);

    // loop through each item in the JSON array
    for (int i = 0; i < tileDbSize; i++) {
        JSON_Object *tileObj = json_array_get_object(typesArray, i);

        tileDatabase[i].id       = (int)json_object_get_number(tileObj, "id");
        tileDatabase[i].walkable     = json_object_get_boolean(tileObj, "walkable");
        tileDatabase[i].interactable = json_object_get_boolean(tileObj, "interactable");
        tileDatabase[i].holdsItem    = json_object_get_boolean(tileObj, "holdsItem");

        const char* fetchedName      = json_object_get_string(tileObj, "name");
        tileDatabase[i].name         = malloc(strlen(fetchedName) + 1);
        strcpy((char*)tileDatabase[i].name, fetchedName);

        const char* textureNameStr   = json_object_get_string(tileObj, "textureName");
        if (textureNameStr != NULL) {
            tileDatabase[i].textureName = malloc(strlen(textureNameStr) + 1);
            strcpy((char*)tileDatabase[i].textureName, textureNameStr);
        } else {
            tileDatabase[i].textureName = NULL;
        }

        tileDatabase[i].fallbackColor = ParseRaylibColor(tileObj);
    }

    // safely clean up parson internal memory tree
    json_value_free(rootValue);
    printf("successfully loaded registry %d tile types indexed.\n", tileDbSize);
}

// load the map
void LoadMapLayout(const char* filename) {
    JSON_Value *rootValue = json_parse_file(filename);
    if (rootValue == NULL) {
        printf("Error: Failed to parse map layout file: %s\n", filename);
        return;
    }

    JSON_Object *rootObject = json_value_get_object(rootValue);
    JSON_Array *layoutArray = json_object_get_array(rootObject, "layout");

    // total tiles is always 144 (16 * 9)
    totalTiles = MAP_ROWS * MAP_COLS;
    levelTiles = malloc(sizeof(MapItem) * totalTiles);

    int tileCounter = 0;

    for (int row = 0; row < MAP_ROWS; row++) {
        JSON_Array *currentRow = json_array_get_array(layoutArray, row);

        for (int col = 0; col < MAP_COLS; col++) {
            levelTiles[tileCounter].xPos   = col;
            levelTiles[tileCounter].yPos   = row;
            levelTiles[tileCounter].tileId = (int)json_array_get_number(currentRow, col);

            tileCounter++;
        }
    }

    json_value_free(rootValue);
    printf("map loaded successfully into fixed %dx%d layout grid\n", MAP_COLS, MAP_ROWS);
}

TileType* GetTileBlueprintAt(int gridX, int gridY) {
    int targetIndex = (gridY * MAP_COLS) + gridX;

    if (targetIndex < 0 || targetIndex >= totalTiles) {
        return NULL;
    }

    // get raw tileId
    int currentId = levelTiles[targetIndex].tileId;

    // scan the tile database to return the matching structural properties
    for (int i = 0; i < tileDbSize; i++) {
        if (tileDatabase[i].id == currentId) {
            return &tileDatabase[i]; // return the result
        }
    }

    return NULL;
}

void UnloadAllMapData(void) {
    printf("Starting map system memory cleanup...\n");

    // free map
    if (levelTiles != NULL) {
        free(levelTiles);
        levelTiles = NULL;
        printf(" -> levelTiles array freed successfully.\n");
    }

    // free tiles
    if (tileDatabase != NULL) {
        free(tileDatabase);
        tileDatabase = NULL;
        tileDbSize = 0;
        printf(" -> tileDatabase blueprints array freed successfully.\n");
    }

    printf("Map data successfully flushed from RAM!\n");
}
