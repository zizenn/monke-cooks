#ifndef GAME
#define GAME

typedef enum {
  LEFT,
  RIGHT,
  UP,
  DOWN
} DIRECTION;

typedef enum {
  WALKABLE, // this is 0
  COUNTER, // this is 1
  STOVE, // this is 2
  FRIDGE // this is 3
} TILE_TYPE;

typedef enum {
  NONE,
  FRIDGE_MENU,
  STOVE_MENU
} MENU_TYPE;

typedef enum {
  EMPTY,
  RICE,
  MILK,
  EGG
} ITEM;

#endif