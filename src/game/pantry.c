#include "external/raylib.h"
#include "external/raygui.h"
#include "game/screens.h"
#include "game/display_screen.h"
#include "minigames/minigame.h"
#include "game/game.h"
#include "game/globals.h"
#include "game/items.h"
#include "stdio.h"
#include "ctype.h"
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

static const int COLUMNS = 8;
static const int ITEM_WIDTH = 84;
static const int ITEM_HEIGHT = 84;
static int pantryCount = 10;

static bool fuzzyFinder(const char* search, const char* name);

// Forward declaration of UnloadPantry
void UnloadPantry();

void InitPantry() {
  // variable setting
  confirm = false;
  searchBarText[0] = '\0';
  searchEditMode = true;
  selectedItem = holding.categoryId;

  totalArea = (Rectangle){640-346.5, 360-178.5, 693, 357};
  panelBounds = (Rectangle){totalArea.x, totalArea.y, totalArea.width, totalArea.height-16};

  // textures - load variant 0 (first) of each category
  for (int i = 0; i < pantryCount; i++) {
    int categoryId = stockedPantry[i].categoryId;
    pantryTextures[i] = LoadTexture(allPantry[categoryId].variants[0].filePath);
  }

  // init the filter at the start
  matchCount = 0;
  for (int i = 0; i < pantryCount; i++) {
    int categoryId = stockedPantry[i].categoryId;
    const char* categoryName = allPantry[categoryId].categoryName;
    if (fuzzyFinder(searchBarText, categoryName)) {
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
    UnloadPantry();
    currentScreen = GAME;
  } else if (IsKeyPressed(KEY_ENTER)) {
    if (matchCount > 0) {
      int selectedStockedIndex = matchedItems[0];
      int categoryId = stockedPantry[selectedStockedIndex].categoryId;
      int variantId = 0;  // Start with first variant
      COOK_TYPE cookType = allPantry[categoryId].variants[variantId].cook_type;
      
      holding = (HoldingItem){ categoryId, variantId, cookType };

      const char *filePath = allPantry[categoryId].variants[variantId].filePath;

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
      UnloadPantry();
      currentScreen = GAME;
    }
  }

  if (strcmp(searchBarText,lastSearchText) != 0) {
    matchCount = 0;

    for (int i = 0; i < pantryCount; i++) {
      int categoryId = stockedPantry[i].categoryId;
      const char* categoryName = allPantry[categoryId].categoryName;
      if (fuzzyFinder(searchBarText, categoryName)) {
        matchedItems[matchCount++] = i;
      }
    }

    strcpy(lastSearchText, searchBarText);
  }
}

void DrawPantry() {
  int screenWidth = GetScreenWidth();
  int screenHeight = GetScreenHeight();
  
  Rectangle panelContent = {0, 0, panelBounds.width-15,  1000};
  Rectangle searchBounds = {totalArea.x, totalArea.y+panelBounds.height, totalArea.width, 16 };
  int startX = panelBounds.x + 5; // padding for each item
  int startY = panelBounds.y + 5; // same as above

  GuiScrollPanel(panelBounds, NULL, panelContent, &scrollOffset,NULL);
  if (GuiTextBox(searchBounds, searchBarText, 64, searchEditMode)) {
    searchEditMode = !searchEditMode;
    // if no matches and textbox was just deselected (enter pressed), close pantry
    if (!searchEditMode && matchCount == 0 && searchBarText[0] != '\0') {
      UnloadPantry();
      currentScreen = GAME;
    }
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
      snprintf(quantityStr, sizeof(quantityStr), "%d", stockedPantry[ingredientIdx].quantity);
      DrawTextureEx(pantryTextures[ingredientIdx], (Vector2){xPos, yPos}, 0, 5.25, WHITE);
      DrawRectangleRec((Rectangle){ xPos, yPos, 20, 20 }, LIGHTGRAY);
      DrawText(quantityStr, xPos+2, yPos+2, 16, BLACK);
      
      // Draw category name below the item with background
      int categoryId = stockedPantry[ingredientIdx].categoryId;
      const char* categoryName = allPantry[categoryId].categoryName;
      int textWidth = MeasureText(categoryName, 10);
      int textX = xPos + (ITEM_WIDTH / 2) - (textWidth / 2);
      int textY = yPos + 68;
      
      // Draw background rectangle
      int padding = 3;
      DrawRectangle(textX - padding, textY - padding, textWidth + (padding * 2), 16, (Color){200, 200, 200, 255});
      DrawRectangleLines(textX - padding, textY - padding, textWidth + (padding * 2), 16, BLACK);
      
      // Draw text on top of background
      DrawText(categoryName, textX, textY, 10, BLACK);
    }
  }

  EndScissorMode();
}

void UnloadPantry() {
  for (int i = 0; i < pantryCount; i++) {
    UnloadTexture(pantryTextures[i]);
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
