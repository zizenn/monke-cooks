#ifndef GAME_H
#define GAME_H

#include "external/raylib.h"
#include "core/types.h"

typedef enum {
  UP,
  DOWN,
  LEFT,
  RIGHT
} DIRECTION;

// player
static const int PLAYER_TEXTURE_COUNT = 4;
extern Holding holding;
extern Texture2D playerTexture[4];
extern Rectangle totalArea;
extern Rectangle panelBounds;

#endif
