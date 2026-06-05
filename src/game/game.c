#include "core/textures.h"
#include "external/raylib.h"
#include "core/scenes.h"
#include "core/map.h"
#include "core/globals.h"
#include "core/config.h"
#include "game/player.h"
#include "stddef.h"

void DrawMapGridLines(void);

void LoadGame(void) {
  // loading stuff
  LoadTileDatabase("assets/mapData/tiles.json");
  LoadMapLayout("assets/mapData/mapFiles/map1.json");
  LoadTextureDatabase("assets/data/textures.json");

  InitPlayer(5, 5);
}

void UpdateGame(void) {
  UpdatePlayer();
}

void DrawGame(void) {
  DrawMapGridLines();
  DrawPlayer();
}

void UnloadGame(void) {
  UnloadAllMapData();
  UnloadTextureDatabase();
}

void DrawMapGridLines(void) {
  for (int col = 0; col <= MAP_COLS; col++) {
    int x = col * TILE_SIZE;
    DrawLine(x, 0, x, VIRTUAL_HEIGHT, DARKGRAY);
  }

  for (int row = 0; row <= MAP_ROWS; row++) {
    int y = row * TILE_SIZE;
    DrawLine(0, y, VIRTUAL_WIDTH, y, DARKGRAY);
  }
}
