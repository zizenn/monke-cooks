#ifndef GAME_H
#define GAME_H

#include "external/raylib.h"
#include "core/types.h"
#include "config.h"

typedef enum {
  UP,
  DOWN,
  LEFT,
  RIGHT
} DIRECTION;

// tiling system
static inline int TileToPixels(int tiles) {
  return tiles * TILE_SIZE;
}

static inline float TilesToPixels(float tiles) {
  return tiles * (float)TILE_SIZE;
}

// player
static const int PLAYER_TEXTURE_COUNT = 4;
extern Holding holding;
extern Texture2D playerTexture[4];

#endif
