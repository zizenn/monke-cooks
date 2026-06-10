#include "game/player.h"
#include "game/keymap.h"
#include "core/map.h"
#include "core/textures.h"
#include "core/config.h"
#include "stddef.h"

// variables setting
Player player;
static float visualX = 0.0f;
static float visualY = 0.0f;
static bool isMoving = false;
static float walkSpeed = 48.0f; // higher number = faster glide speed

// functoin prototypes
float float_abs(float value);

void InitPlayer(int startX, int startY) {
    player.gridX = startX;
    player.gridY = startY;
    player.sprite = "PLAYER_DOWN";

    visualX = (float)(startX * TILE_SIZE);
    visualY = (float)(startY * TILE_SIZE);
}

void UpdatePlayer(void) {
  float targetPixelX = (float)(player.gridX * TILE_SIZE);
  float targetPixelY = (float)(player.gridY * TILE_SIZE);

  if (isMoving) {
    visualX += (targetPixelX - visualX) * walkSpeed * GetFrameTime();
    visualY += (targetPixelY - visualY) * walkSpeed * GetFrameTime();

    if (float_abs(targetPixelX - visualX) < 0.5f && float_abs(targetPixelY - visualY) < 0.5f) {
      visualX = targetPixelX;
      visualY = targetPixelY;
      isMoving = false;
    }
    return;
  }

  int nextX = player.gridX;
  int nextY = player.gridY;
  bool intentToMove = false;

  if (IsActionPressed(ACTION_UP)) {
    nextY--;
    player.sprite = "PLAYER_UP";
    intentToMove = true;
  }
  else if (IsActionPressed(ACTION_DOWN)) {
    nextY++;
    player.sprite = "PLAYER_DOWN";
    intentToMove = true;
  }
  else if (IsActionPressed(ACTION_LEFT)) {
    nextX--;
    player.sprite = "PLAYER_LEFT";
    intentToMove = true;
  }
  else if (IsActionPressed(ACTION_RIGHT)) {
    nextX++;
    player.sprite = "PLAYER_RIGHT";
    intentToMove = true;
  }

  if (intentToMove) {
    // bounds check
    if (nextX < 0 || nextX >= MAP_COLS || nextY < 0 || nextY >= MAP_ROWS) {
      return;
    }

    // collision check
    TileType* targetBlueprint = GetTileBlueprintAt(nextX, nextY);
    if (targetBlueprint != NULL && !targetBlueprint->walkable) {
      return;
    }

    player.gridX = nextX;
    player.gridY = nextY;
    isMoving = true;
  }
}

void DrawPlayer(void) {
  Texture2D currentTex = GetTextureByName(player.sprite);

  if (currentTex.id == 0) return;

  Rectangle sourceRect = {
    0.0f,
    0.0f,
    (float)currentTex.width,
    (float)currentTex.height
  };

  Rectangle destRect = {
    visualX,
    visualY,
    (float)TILE_SIZE,
    (float)TILE_SIZE
  };

  Vector2 origin = { 0.0f, 0.0f };

  DrawTexturePro(currentTex, sourceRect, destRect, origin, 0.0f, WHITE);
}

float float_abs(float value) {
  return (value < 0.0f) ? -value : value;
}
