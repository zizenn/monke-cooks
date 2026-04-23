#ifndef GAME_H
#define GAME_H

#include "external/raylib.h"
#include "game/items.h"

typedef enum {
  LEFT,
  RIGHT,
  UP,
  DOWN
} DIRECTION;

typedef enum {
  FROM_FRIDGE,
  FROM_PANTRY,
  FROM_STOVE,
  FROM_OVEN,
  FROM_DEEP_FRY,
  FROM_GRILL,
  FROM_NONE
} whereIsItemFrom;

extern whereIsItemFrom itemFrom;
void quantityLower(whereIsItemFrom type);

// tiling system
enum {
  TILE_SIZE = 64,
  VISIBLE_ROWS = 9,
  mapRows = 9,
  mapCols = 16,
  VIRTUAL_WIDTH = 1024,
  VIRTUAL_HEIGHT = 576
};

static inline int TileToPixels(int tiles) {
  return tiles * TILE_SIZE;
}

static inline float TilesToPixels(float tiles) {
  return tiles * (float)TILE_SIZE;
}

// player
static const int PLAYER_TEXTURE_COUNT = 4;
extern ItemType holding;
extern Texture2D playerTexture[PLAYER_TEXTURE_COUNT];

#endif
