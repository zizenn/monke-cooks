#include "game/level.h"
#include "core/textures.h"
#include "external/parson.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Level* currentLevel = NULL;
static int currentLevelIndex = -1;
Level* GameLevels = NULL;
int TotalGameLevels = 0;

void GenericLevelLoad(Level* self);
void GenericLevelUpdate(Level* self);
void GenericLevelDraw(Level* self);
void GenericLevelUnload(Level* self);

void LoadLevelManifest(const char* filename) {
  JSON_Value *rootValue = json_parse_file(filename);
  if (rootValue == NULL) {
    printf("Error: Engine failed to parse level manifest file at: %s\n", filename);
    return;
  }

  JSON_Object *rootObject = json_value_get_object(rootValue);
  JSON_Array *levelsArray = json_object_get_array(rootObject, "levels");

  TotalGameLevels = (int)json_array_get_count(levelsArray);

  GameLevels = malloc(sizeof(Level) * TotalGameLevels);

  for (int i = 0; i < TotalGameLevels; i++) {
    JSON_Object *levelObj = json_array_get_object(levelsArray, i);

    GameLevels[i].id = (int)json_object_get_number(levelObj, "id");

    const char* nameStr = json_object_get_string(levelObj, "name");
    GameLevels[i].name = malloc(strlen(nameStr) + 1);
    strcpy(GameLevels[i].name, nameStr);

    const char* mapPath = json_object_get_string(levelObj, "mapJsonPath");
    GameLevels[i].mapJsonPath = malloc(strlen(mapPath) + 1);
    strcpy(GameLevels[i].mapJsonPath, mapPath);

    const char* tilePath = json_object_get_string(levelObj, "tileManifestJsonPath");
    GameLevels[i].tileManifestJsonPath = malloc(strlen(tilePath) + 1);
    strcpy(GameLevels[i].tileManifestJsonPath, tilePath);

    const char* texPath = json_object_get_string(levelObj, "textureJsonPath");
    GameLevels[i].textureJsonPath = malloc(strlen(texPath) + 1);
    strcpy(GameLevels[i].textureJsonPath, texPath);

    GameLevels[i].Load = GenericLevelLoad;
    GameLevels[i].Update = GenericLevelUpdate;
    GameLevels[i].Draw = GenericLevelDraw;
    GameLevels[i].Unload = GenericLevelUnload;
  }

  json_value_free(rootValue);
  printf("Level Pipeline: Successfully cached %d layout indexes to memory registry.\n", TotalGameLevels);
}

void UnloadLevelManifest(void) {
  if (GameLevels != NULL) {
    for (int i = 0; i < TotalGameLevels; i++) {
      free(GameLevels[i].name);
      free(GameLevels[i].mapJsonPath);
      free(GameLevels[i].tileManifestJsonPath);
      free(GameLevels[i].textureJsonPath);
    }
    free(GameLevels);
    GameLevels = NULL;
    TotalGameLevels = 0;
    printf("Level Pipeline: Dynamic manifest array flushed from memory completely.\n");
  }
}

void SwitchToLevelByIndex(int humanIndex) {
  int arrayIndex = humanIndex - 1;

  if (arrayIndex < 0 || arrayIndex >= TotalGameLevels) {
    printf("Scene Warning: Level %d does not exist. Ignoring switch.\n", humanIndex);
    return;
  }

  if (currentLevel != NULL) {
    printf("Scene Manager: Cleaning up old level -> %s\n", currentLevel->name);
    if (currentLevel->Unload != NULL) {
      currentLevel->Unload(currentLevel);
    }
    UnloadLevelTextureDatabase();
  }

  currentLevelIndex = arrayIndex;
  currentLevel = &GameLevels[currentLevelIndex];

  printf("Scene Manager: Entering Level %d -> %s\n", currentLevel->id, currentLevel->name);

  InitLevelTextureDatabase(currentLevel->textureJsonPath);
}

void SwitchToNextLevel(void) {
  SwitchToLevelByIndex(currentLevelIndex + 2); // +2 because human index is array + 1
}
