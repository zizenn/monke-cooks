#include "external/raylib.h"
#include "external/raygui.h"
#include "game/screens.h"
#include "game/display_screen.h"
#include "game/globals.h"
#include "game/game.h"
#include "game/items.h"
#include "stdio.h"
#include "stdlib.h"

ITEM stockedItems[] = {
  MILK,
  RICE,
  EGG
};

// function prototypes


// variables
static bool confirm;

void InitFridge() {
  confirm = false;
}

void UpdateFridge() {

}

void DrawFridge() {
  int screenWidth = GetScreenWidth();
  int screenHeight = GetScreenHeight();

  BeginTextureMode(canvas);
  ClearBackground(WHITE);

  Rectangle background = {50, 50, 924, 476};
  Rectangle fridgeRecs[] = {
    {10, 20, 10, 10} // item 1
  };

  DrawRectangleRec(background, LIGHTGRAY);
  
  EndTextureMode();

  Rectangle source = {0, 0, canvas.texture.width, -canvas.texture.height };
  Rectangle dest = { 0, 0, screenWidth, screenHeight };
  Vector2 origin = { 0, 0 };

  // this shouldnt be changed unless u really need to which wont happen (this basically draws the whole BeginTextureMode but scales it so it works)
  DrawTexturePro(canvas.texture, source, dest, origin, 0.0f, WHITE);
}

void UnloadFridge() {
  // add textures here when i get textures for this
}
