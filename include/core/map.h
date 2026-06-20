#ifndef MAP_H
#define MAP_H

#define MAP_WIDTH 16
#define MAP_HEIGHT 9

#include "external/raylib.h"
#include "stdbool.h"

typedef enum {
  TILE_FLOOR,
  TILE_COUNTER,
  TILE_SINK,
  TILE_STOVE
} TILE_ID;

typedef void (*funcPointer)(void);

typedef struct {
  TILE_ID id;
  const char* name;
  bool walkable;
  int actionId;
  char* textureName;
  Color fallbackColor;
} TileType;

typedef struct {
  int xPos;
  int yPos;
  TILE_ID tileId;
} MapItem;

// tiles
extern TileType* tileDatabase;
extern int tileDbSize;
extern MapItem* levelTiles;
extern int totalTiles;

// actions LUT


// function prototypes
void LoadTileDatabase(const char* filename);
void LoadMapLayout(const char* filename);
void UnloadAllMapData(void);
TileType* GetTileBlueprintAt(int gridX, int gridY);

#endif
