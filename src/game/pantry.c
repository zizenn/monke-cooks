#include "external/raylib.h"
#include "external/raygui.h"
#include "game/screens.h"
#include "game/display_screen.h"
#include "minigames/minigame.h"
#include "game/game.h"
#include "game/globals.h"
#include "game/items.h"
#include "game/texture_cache.h"
#include "stdio.h"
#include "ctype.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"

// textures
static const int PANTRY_ITEM_COUNT = 10;
static Texture2D pantryTextures[10] = {};

// vector2s
static Vector2 scrollOffset = {0, 0};

// variables
static bool confirm;
static char searchBarText[64] = "";
static bool searchEditMode = false;
static int matchedItems[10];
static int matchCount = 0;
static char lastSearchText[64] = "";
static int selectedItem;
static char lowerCategoryNames[10][65] = {0};

static const int COLUMNS = 8;
static const int ITEM_WIDTH = 84;
static const int ITEM_HEIGHT = 84;
static int pantryCount = 10;

static bool FuzzyFinder(const char* search, const char* name);
static void ToLowerString(const char* source, char* output, int outputSize);

void InitPantry() {
  // variable setting
  confirm = false;
  searchBarText[0] = '\0';
  searchEditMode = true;
  selectedItem = holding.categoryId;

  UpdateUILayoutRects();

  // textures - load variant 0 (first) of each category
  for (int i = 0; i < pantryCount; i++) {
    int categoryId = stockedPantry[i].categoryId;
    pantryTextures[i] = LoadTexture(allPantry[categoryId].variants[0].filePath);
    ToLowerString(allPantry[categoryId].categoryName, lowerCategoryNames[i], 65);
  }

  // init the filter at the start
  matchCount = 0;
  for (int i = 0; i < pantryCount; i++) {
    int categoryId = stockedPantry[i].categoryId;
    if (FuzzyFinder(searchBarText, lowerCategoryNames[i])) {
      matchedItems[matchCount++] = i;
    }
  }
  strcpy(lastSearchText, searchBarText);

  // raygui
  GuiSetStyle(SCROLLBAR, SCROLLBAR_WIDTH, 3);
}

void UpdatePantry() {
  if (IsKeyPressed(KEY_ESCAPE)) {
    searchEditMode = false;
    currentScreen = GAME;
  } else if (IsKeyPressed(KEY_ENTER)) {
    if (matchCount > 0) {
      int selectedStockedIndex = matchedItems[0];
      int categoryId = stockedPantry[selectedStockedIndex].categoryId;
      int variantId = 0;  // Start with first variant
      COOK_TYPE cookType = allPantry[categoryId].variants[variantId].cook_type;
      PREP_TYPE prepType = allPantry[categoryId].variants[variantId].prep_type;
      
      itemFrom = FROM_PANTRY;
      holding = (ItemType){ categoryId, variantId, cookType };
      currentPrepType = prepType;

      const char *filePath = allPantry[categoryId].variants[variantId].filePath;

      ReleaseTextureArray(playerTexture, 4);
      Texture2D newTexture = AcquireCachedTexture(filePath);
      FillTextureArray(playerTexture, 4, newTexture);

      searchEditMode = false;
      currentScreen = GAME;
    }
  }

  if (strcmp(searchBarText,lastSearchText) != 0) {
    matchCount = 0;

    for (int i = 0; i < pantryCount; i++) {
      int categoryId = stockedPantry[i].categoryId;
      if (FuzzyFinder(searchBarText, lowerCategoryNames[i])) {
        matchedItems[matchCount++] = i;
      }
    }

    strcpy(lastSearchText, searchBarText);
  }
}

void DrawPantry() {
  Rectangle panelContent = {0, 0, panelBounds.width-15,  1000};
  Rectangle searchBounds = {totalArea.x, totalArea.y+panelBounds.height, totalArea.width, 16 };
  int startX = panelBounds.x + 5;
  int startY = panelBounds.y + 5;
  int maxWidth = panelBounds.width - 20;
  int itemsPerRow = maxWidth / ITEM_WIDTH;
  if (itemsPerRow < 1) itemsPerRow = 1;

  GuiScrollPanel(panelBounds, NULL, panelContent, &scrollOffset,NULL);
  if (GuiTextBox(searchBounds, searchBarText, 64, searchEditMode)) {
    searchEditMode = !searchEditMode;
    if (!searchEditMode && matchCount == 0 && searchBarText[0] != '\0') {
      currentScreen = GAME;
    }
  }

  BeginScissorMode(panelBounds.x, panelBounds.y, panelBounds.width, panelBounds.height);

  int totalRows = (matchCount + itemsPerRow - 1) / itemsPerRow;
  panelContent.height = totalRows * ITEM_HEIGHT;
 
  if (matchCount != 0) {
    for (int i = 0; i < matchCount; i++) {
      int ingredientIdx = matchedItems[i];
      int row = i / itemsPerRow;
      int col = i % itemsPerRow;

      float xPos = startX + (col * ITEM_WIDTH);
      float yPos = startY + (row * ITEM_HEIGHT) - scrollOffset.y;

      char quantityStr[10];
      snprintf(quantityStr, sizeof(quantityStr), "%d", stockedPantry[ingredientIdx].quantity);
      DrawTextureEx(pantryTextures[ingredientIdx], (Vector2){xPos, yPos}, 0, 5.25, WHITE);
      DrawRectangleRec((Rectangle){ xPos, yPos, 20, 20 }, LIGHTGRAY);
      DrawText(quantityStr, xPos+2, yPos+2, 16, BLACK);
      
      int categoryId = stockedPantry[ingredientIdx].categoryId;
      const char* categoryName = allPantry[categoryId].categoryName;
      int textWidth = MeasureText(categoryName, 10);
      int textX = xPos + (ITEM_WIDTH / 2.0f) - (textWidth / 2.0f);
      int textY = yPos + 68;
      
      int padding = 3;
      DrawRectangle(textX - padding, textY - padding, textWidth + (padding * 2), 16, (Color){200, 200, 200, 255});
      DrawRectangleLines(textX - padding, textY - padding, textWidth + (padding * 2), 16, BLACK);
      
      DrawText(categoryName, textX, textY, 10, BLACK);
    }
  }

  EndScissorMode();
}

void UnloadPantry() {
  for (int i = 0; i < pantryCount; i++) {
    if (pantryTextures[i].id != 0) {
      UnloadTexture(pantryTextures[i]);
      pantryTextures[i] = (Texture2D){0};
    }
  }
}

static bool FuzzyFinder(const char* search, const char* name) {
  if (search[0] == '\0') return true;

  char lowerSearch[65];

  int i = 0;
  while (search[i]) {
    lowerSearch[i] = tolower(search[i]);
    i++;
  }
  lowerSearch[i] = '\0';

  return strstr(name, lowerSearch) != NULL;
}

static void ToLowerString(const char* source, char* output, int outputSize) {
  int i = 0;
  for (; source[i] != '\0' && i < outputSize - 1; i++) {
    output[i] = (char)tolower((unsigned char)source[i]);
  }
  output[i] = '\0';
}
