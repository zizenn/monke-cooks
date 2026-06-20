#include "core/textures.h"
#include "external/raylib.h"
#include "external/rlgl.h"
#include "game/level.h"
#include "game/player.h"
#include "core/config.h"
#include "core/map.h"
#include "stddef.h"
#include "stdio.h"

// variables
RenderTexture2D tileRenderTexture;
static bool mapNeedsRedraw = true;
Font jetbrainsmonoRegular;

// function prototypes
void DrawMapGridLines(void);
static void DrawTiles(void);
static funcPointer SearchLUTForAction(int actionId);
static void PerformActions(int actionId);

void GenericLevelLoad(Level* self) {
  printf("game: loading level: %s\n", self->name);
  InitPlayer(1, 1);
  LoadTileDatabase(self->tileManifestJsonPath);
  LoadMapLayout(self->mapJsonPath);
  InitLevelTextureDatabase(self->textureJsonPath);
  tileRenderTexture = LoadRenderTexture(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
}

void GenericLevelUpdate(Level* self) {
  (void)self;
  if (IsKeyPressed(KEY_R)) {
    mapNeedsRedraw = true;
  }
  if (mapNeedsRedraw) {
    DrawTiles();
    mapNeedsRedraw = false;
  }

  UpdatePlayer();
}

void GenericLevelDraw(Level* self) {
  (void)self;

  DrawTextureRec(tileRenderTexture.texture,
    (Rectangle){ 0, 0, (float)tileRenderTexture.texture.width, -(float)tileRenderTexture.texture.height },
    (Vector2){ 0, 0 },
    WHITE);

  DrawMapGridLines();
  DrawPlayer();
}

void GenericLevelUnload(Level* self) {
  UnloadAllMapData();
  UnloadFont(jetbrainsmonoRegular);
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

static void DrawTiles(void) {
  BeginTextureMode(tileRenderTexture);
  ClearBackground(BLANK);

  while (!IsTextureDatabaseReady()) {
    // wait for the texture database to be ready before drawing tiles
  }

  for (int i = 0; i < totalTiles; i++) {
    MapItem currentTile = levelTiles[i];

    int pixelX = currentTile.xPos * TILE_SIZE;
    int pixelY = currentTile.yPos * TILE_SIZE;

    TileType* tileBlueprint = GetTileBlueprintAt(currentTile.xPos, currentTile.yPos);
    if (tileBlueprint != NULL) {
      if (tileBlueprint->textureName != NULL && tileBlueprint->textureName[0] != '\0') {
        Texture2D texture = GetTextureByName(tileBlueprint->textureName);
        DrawTexture(texture, pixelX, pixelY, WHITE);
      } else {
        DrawRectangle(pixelX, pixelY, TILE_SIZE, TILE_SIZE, tileBlueprint->fallbackColor);
      }
    }
  }

  EndTextureMode();

  printf("tiling system: tiles updated");
}
