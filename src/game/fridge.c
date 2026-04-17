#include "external/raylib.h"
#include "external/raygui.h"
#include "game/screens.h"
#include "game/display_screen.h"
#include "game/globals.h"
#include "game/game.h"
#include "game/items.h"
#include "stdio.h"
#include "stdlib.h"

// defining stuff
ITEM stockedItems[] = {
  MILK,
  RICE,
  EGG
};

// textures
Texture2D foodTextures[] = {};

// rectangles || vector2s
Rectangle background = {50, 50, 924, 476};

Vector2 foodPositions[] = {
{75,  75}, // item 1
{75,  75}, // item 1
{75,  75}, // item 1
};

// function prototypes


// variables
static bool confirm;

void InitFridge() {
  confirm = false;

  // textures
  foodTextures[0] = LoadTexture("assets/food/38_friedegg.png");
  foodTextures[1] = LoadTexture("assets/food/18_burrito.png");
}

void UpdateFridge() {

}

void DrawFridge() {
  int screenWidth = GetScreenWidth();
  int screenHeight = GetScreenHeight();

  Rectangle source = {0, 0, canvas.texture.width, -canvas.texture.height };
  Rectangle dest = { 0, 0, screenWidth, screenHeight };
  Vector2 origin = { 0, 0 };

  BeginTextureMode(canvas);

  ClearBackground(WHITE);

  DrawRectangleRec(background, LIGHTGRAY);
  DrawTextureEx(foodTextures[0], foodPositions[0], 0, 2, WHITE);

  EndTextureMode();

  
  // this shouldnt be changed unless u really need to which wont happen (this basically draws the whole BeginTextureMode but scales it so it works)
  DrawTexturePro(canvas.texture, source, dest, origin, 0.0f, WHITE);
}

void UnloadFridge() {
  // add textures here when i get textures for this
  UnloadTexture(foodTextures[0]);
  UnloadTexture(foodTextures[1]);
}
