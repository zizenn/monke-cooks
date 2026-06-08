#include "core/textures.h"
#include "external/raylib.h"
#include "game/level.h"
#include "game/player.h"
#include "core/config.h"
#include "core/map.h"
#include "stddef.h"
#include "stdio.h"

// function prototypes
void DrawMapGridLines(void);

void GenericLevelLoad(Level* self) {
  printf("game: loading level: %s\n", self->name);
  InitPlayer(1, 1);
  LoadTileDatabase(self->tileManifestJsonPath);
  LoadMapLayout(self->mapJsonPath);
}

void GenericLevelUpdate(Level* self) {
  (void)self;
  UpdatePlayer();
}

static void DrawTile(Texture2D tex, int pixelX, int pixelY, Color fallbackColor) {
  if (tex.id != 0) {
    Rectangle source = { 0.0f, 0.0f, (float)tex.width, (float)tex.height };
    Rectangle dest = { (float)pixelX, (float)pixelY, (float)TILE_SIZE, (float)TILE_SIZE };
    DrawTexturePro(tex, source, dest, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
  } else {
    DrawRectangle(pixelX, pixelY, TILE_SIZE, TILE_SIZE, fallbackColor);
  }
}

void GenericLevelDraw(Level* self) {
  (void)self;

  for (int i = 0; i < totalTiles; i++) {
    MapItem currentTile = levelTiles[i];

    int pixelX = currentTile.xPos * TILE_SIZE;
    int pixelY = currentTile.yPos * TILE_SIZE;

    if (currentTile.tileId == TILE_COUNTER) {
      Texture2D counterTex = GetTextureByName("COUNTER_TOP");
      DrawTile(counterTex, pixelX, pixelY, LIGHTGRAY);
    }
    else if (currentTile.tileId == TILE_SINK) {
      Texture2D sinkTex = GetTextureByName("SINK_TOP");
      DrawTile(sinkTex, pixelX, pixelY, BLUE);
    }
    else {
      // Default fallback layer: TILE_FLOOR
      Texture2D floorTex = GetTextureByName("FLOOR_TILE");
      DrawTile(floorTex, pixelX, pixelY, RAYWHITE);
    }
  }

  DrawMapGridLines();
  DrawPlayer();
}

void GenericLevelUnload(Level* self) {
  UnloadAllMapData();
  printf("game: unloaded level: %s\n", self->name);
}

void DrawMapGridLines(void) {
  for (int col = 0; col <= MAP_COLS; col++) {
    int x = col * TILE_SIZE;
    DrawLineEx((Vector2){x, 0}, (Vector2){x, VIRTUAL_HEIGHT}, 1.0f, DARKGRAY);
  }

  for (int row = 0; row <= MAP_ROWS; row++) {
    int y = row * TILE_SIZE;
    DrawLineEx((Vector2){0, y}, (Vector2){VIRTUAL_WIDTH, y}, 1.0f, DARKGRAY);
  }
}
