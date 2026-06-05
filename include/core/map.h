#ifndef MAP_H
#define MAP_H

#include "stdbool.h"

typedef enum {
  TILE_FLOOR,
  TILE_COUNTER,
  TILE_SINK
} TILE_ID;

typedef struct {
  TILE_ID id;
  const char* name;
  bool walkable;
  bool interactable;
  bool holdsItem;
  const char* filePath; // Raylib texture loaded at runtime
} TileType;

typedef struct {
  int xPos;
  int yPos;
  TILE_ID tileId;
} MapItem;

extern TileType* tileDatabase;
extern int tileDbSize;
extern MapItem* levelTiles;
extern int totalTiles;

// function prototypes
void LoadTileDatabase(const char* filename);
void LoadMapLayout(const char* filename);
void UnloadAllMapData(void);
TileType* GetTileBlueprintAt(int gridX, int gridY);

#endif
