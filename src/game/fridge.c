#include "external/raylib.h"
#include "external/raygui.h"
#include "game/screens.h"
#include "game/display_screen.h"
#include "game/game.h"
#include "game/globals.h"
#include "game/items.h"
#include "stdio.h"
#include "ctype.h"
#include "stdbool.h"
#include "string.h"

// defining stuff
stockItemIngredient stockedIngredients[] = {
  { RAW_EGG, 2 },
  { RAW_RICE, 1 },
  { RAW_SHIITAKE, 3 }
};

// textures
static Texture2D foodTextures[sizeof(stockedIngredients) / sizeof(stockedIngredients[0])] = {};

// vector2s

static Vector2 scrollOffset = {0, 0};

// variables
static bool confirm;
static char searchBarText[64] = "";
static bool searchEditMode = false;
static int matchedItems[sizeof(stockedIngredients) / sizeof(stockedIngredients[0])];
static int matchCount = 0;
static char lastSearchText[64] = "";
static int selectedItem;

static const int COLUMNS = 8;
static const int ITEM_WIDTH = 84;
static const int ITEM_HEIGHT = 84;
static int ingredientCount = sizeof(stockedIngredients) / sizeof(stockedIngredients[0]);

static bool fuzzyFinder(const char* search, const char* name);

void InitFridge() {
  // variable setting
  confirm = false;
  searchBarText[0] = '\0';
  searchEditMode = true;
  selectedItem = holding;
  // textures
  for (int i = 0; i < ingredientCount; i++) {
    foodTextures[i] = LoadTexture(allIngredients[stockedIngredients[i].foodIndex].filePath);
  }

  // init the filter at the start
  matchCount = 0;
  for (int i = 0; i < ingredientCount; i++) {
    int foodIndex = stockedIngredients[i].foodIndex;
    if (fuzzyFinder(searchBarText, allIngredients[foodIndex].name)) {
      matchedItems[matchCount++] = i;
    }
  }
  strcpy(lastSearchText, searchBarText);

  // raygui
  GuiSetStyle(SCROLLBAR, SCROLLBAR_WIDTH, 3);
}

void UpdateFridge() {
  if (IsKeyPressed(KEY_ESCAPE)) {
    searchEditMode = false;
    UnloadFridge();
    currentScreen = GAME;
  } else if (IsKeyPressed(KEY_ENTER)) {
    int selectedStockedIndex = matchedItems[0];
    int selectedIngredientIndex = stockedIngredients[selectedStockedIndex].foodIndex;
    holding = (ITEM)selectedIngredientIndex;

    const char *filePath = allIngredients[selectedIngredientIndex].filePath;

    // unloading the textures
    for (int i = 0; i < 4; i++) {
      if (playerTexture[i].id != 0) {
        UnloadTexture(playerTexture[i]);
      }
    }

    // loading the new textures
    for (int i = 0; i < 4; i++) {
      playerTexture[i] = LoadTexture(filePath);
    }

    searchEditMode = false;
    UnloadFridge();
    currentScreen = GAME;
  }

  if (strcmp(searchBarText,lastSearchText) != 0) {
    matchCount = 0;

    for (int i = 0; i < ingredientCount; i++) {
      int foodIndex = stockedIngredients[i].foodIndex;
      if (fuzzyFinder(searchBarText, allIngredients[foodIndex].name)) {
        matchedItems[matchCount++] = i;
      }
    }

    strcpy(lastSearchText, searchBarText);
  }
}

void DrawFridge() {
  int screenWidth = GetScreenWidth();
  int screenHeight = GetScreenHeight();
  
  Rectangle totalArea = {(screenWidth/2)-346.5, (screenHeight/2)-178.5, 693, 357};
  Rectangle panelBounds = {totalArea.x, totalArea.y, totalArea.width, totalArea.height-16};
  Rectangle panelContent = {0, 0, panelBounds.width-15,  1000};
  Rectangle searchBounds = {totalArea.x, totalArea.y+panelBounds.height, totalArea.width, 16 };
  int startX = panelBounds.x + 5; // padding for each item
  int startY = panelBounds.y + 5; // same as above

  GuiScrollPanel(panelBounds, NULL, panelContent, &scrollOffset,NULL);
  if (GuiTextBox(searchBounds, searchBarText, 64, searchEditMode)) {
    searchEditMode = !searchEditMode;
  }

  BeginScissorMode(panelBounds.x, panelBounds.y, panelBounds.width, panelBounds.height);

  int totalRows = (matchCount + COLUMNS - 1) / COLUMNS;
  panelContent.height = totalRows * ITEM_HEIGHT;
 
  if (matchCount != 0) {
    for (int i = 0; i < matchCount; i++) {
      int ingredientIdx = matchedItems[i];
      int row = i / COLUMNS;
      int col = i % COLUMNS;

      float xPos = startX + (col * ITEM_WIDTH);
      float yPos = startY + (row * ITEM_HEIGHT) - scrollOffset.y;

      char quantityStr[10];
      snprintf(quantityStr, sizeof(quantityStr), "%d", stockedIngredients[ingredientIdx].quantity);
      DrawTextureEx(foodTextures[ingredientIdx], (Vector2){xPos, yPos}, 0, 5.25, WHITE);
      DrawRectangleRec((Rectangle){ xPos, yPos, 20, 20 }, LIGHTGRAY);
      DrawText(quantityStr, xPos+2, yPos+2, 16, BLACK);
      }
  }

  EndScissorMode();
}

void UnloadFridge() {
  for (int i = 0; i < ingredientCount; i++) {
    UnloadTexture(foodTextures[i]);
  }
}

static bool fuzzyFinder(const char* search, const char* name) {
  if (search[0] == '\0') return true;

  char lowerSearch[65], lowerName[65];

  int i = 0;
  while (search[i]) {
    lowerSearch[i] = tolower(search[i]);
    i++;
  }
  lowerSearch[i] = '\0';

  i = 0;
  while (name[i]) {
    lowerName[i] = tolower(name[i]);
    i++;
  }
  lowerName[i] = '\0';

  return strstr(lowerName, lowerSearch) != NULL;
}
