#ifndef GAME
#define GAME

typedef enum {
  LEFT,
  RIGHT,
  UP,
  DOWN
} DIRECTION;

// tiling system
enum {
  TILE_SIZE = 64,
  VISIBLE_ROWS = 9,
  mapRows = 9,
  mapCols = 16,
  VIRTUAL_WIDTH = TILE_SIZE * 16,
  VIRTUAL_HEIGHT = TILE_SIZE * 9
};

static inline int TileToPixels(int tiles) {
  return tiles * TILE_SIZE;
}

static inline float TilesToPixels(float tiles) {
  return tiles * (float)TILE_SIZE;
}

#endif
