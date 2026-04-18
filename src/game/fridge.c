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
inventorySlot stockedItems[] = {
  {EGG, 2}
};

// textures
Texture2D foodTextures[3] = {};

// rectangles || vector2s

static Vector2 foodPositions[] = {
{75,  75}, // item 1
{175,  75}, // item 2
{275,  75}, // item 3
};

static Vector2 scrollOffset = {0, 0};

// function prototypes


// variables
static bool confirm;
static char searchBarText[64] = "";
static bool searchEditMode = false;

void InitFridge() {
  // variable setting
  confirm = false;
  searchBarText[0] = '\0';
  searchEditMode = true;

  // textures
  foodTextures[0] = LoadTexture("assets/food/38_friedegg.png");
  foodTextures[1] = LoadTexture("assets/food/18_burrito.png");
  foodTextures[2] = LoadTexture("assets/food/09_baguette.png");

  // raygui
  GuiSetStyle(SCROLLBAR, SCROLLBAR_WIDTH, 3);
}

void UpdateFridge() {
  if (IsKeyPressed(KEY_ENTER)) {
    searchEditMode = false;
  }
}

void DrawFridge() {
  int screenWidth = GetScreenWidth();
  int screenHeight = GetScreenHeight();
  
  Rectangle totalArea = {(screenWidth/2)-346.5, (screenHeight/2)-178.5, 693, 357};
  Rectangle panelBounds = {totalArea.x, totalArea.y, totalArea.width, totalArea.height-16};
  Rectangle panelContent = {0, 0, panelBounds.width-15,  1000};
  Rectangle searchBounds = {totalArea.x, totalArea.y+panelBounds.height, totalArea.width, 16 };

  GuiScrollPanel(panelBounds, NULL, panelContent, &scrollOffset,NULL);
  if (GuiTextBox(searchBounds, searchBarText, 64, searchEditMode)) {
    searchEditMode = !searchEditMode;
  }
}

void UnloadFridge() {
  // add textures here when i get textures for this
  UnloadTexture(foodTextures[0]);
  UnloadTexture(foodTextures[1]);
  UnloadTexture(foodTextures[2]);
}
