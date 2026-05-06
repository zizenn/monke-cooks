#include "external/raylib.h"
#include "external/raygui.h"
#include "game/display_screen.h"
#include "game/globals.h"
#include "game/game.h"
#include "game/items.h"
#include "game/texture_manager.h"
#include "game/config.h"
#include "minigames/minigame.h"
#include "stdio.h"
#include "ctype.h"
#include "stdbool.h"
#include "string.h"

// textures - static array mapping category index to texture ID
static const TextureID pantryCategoryTextures[PANTRY_ITEM_COUNT] = {
  TURMERIC_RAW,
  VANILLA_RAW,
  STAR_ANISE_WHOLE,
  CLOVES_WHOLE,
  NUTMEG_WHOLE,
  WHEAT_RAW,
  BARLEY_RAW,
  NORI_DRIED,
  WAKAME_DRIED,
  MILK_FRESH,
};

static Texture2D pantryTextures[PANTRY_ITEM_COUNT] = {};

// vector2s
static Vector2 scrollOffset = {0, 0};

// variables
static bool confirm;
static char searchBarText[64] = "";
static bool searchEditMode = false;
static int matchedItems[PANTRY_ITEM_COUNT];
static int matchCount = 0;
static char lastSearchText[INVENTORY_SEARCH_BOX_MAX_LENGTH] = "";
static int selectedItem;
static char lowerCategoryNames[PANTRY_ITEM_COUNT][INVENTORY_CATEGORY_NAME_MAX_LENGTH] = {0};

static const int COLUMNS = INVENTORY_COLUMNS;
static const int ITEM_WIDTH = INVENTORY_ITEM_WIDTH;
static const int ITEM_HEIGHT = INVENTORY_ITEM_HEIGHT;
static int pantryCount = PANTRY_ITEM_COUNT;

static bool FuzzyFinder(const char* search, const char* name);
static void ToLowerString(const char* source, char* output, int outputSize);

void InitPantry() {
  // variable setting
  confirm = false;
  searchBarText[0] = '\0';
  searchEditMode = true;
  selectedItem = holding.categoryId;

  // textures - get preloaded textures from texture manager
  for (int i = 0; i < pantryCount; i++) {
    int categoryId = stockedPantry[i].categoryId;
    pantryTextures[i] = GetTexture(pantryCategoryTextures[categoryId]);
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
      
      holding = (Holding){ categoryId, variantId, cookType, FROM_PANTRY, ARRAY_FOOD };
      currentPrepType = prepType;

      TraceLog(LOG_INFO, "category id: %d  |  variant id: %d  |  item name: %s", categoryId, variantId, allPantry[categoryId].variants->name);

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
  if (GuiTextBox(searchBounds, searchBarText, INVENTORY_SEARCH_BOX_MAX_LENGTH, searchEditMode)) {
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
      DrawTextureEx(pantryTextures[ingredientIdx], (Vector2){xPos, yPos}, 0, INVENTORY_TEXTURE_SCALE, WHITE);
      DrawRectangleRec((Rectangle){ xPos, yPos, INVENTORY_QUANTITY_BOX_WIDTH, INVENTORY_QUANTITY_BOX_HEIGHT }, LIGHTGRAY);
      DrawText(quantityStr, xPos+2, yPos+2, INVENTORY_QUANTITY_BOX_FONT_SIZE, BLACK);
      
      int categoryId = stockedPantry[ingredientIdx].categoryId;
      const char* categoryName = allPantry[categoryId].categoryName;
      int textWidth = MeasureText(categoryName, INVENTORY_LABEL_FONT_SIZE);
      int textX = xPos + (ITEM_WIDTH / 2.0f) - (textWidth / 2.0f);
      int textY = yPos + 68;
      
      int padding = INVENTORY_LABEL_PADDING;
      DrawRectangle(textX - padding, textY - padding, textWidth + (padding * 2), 16, INVENTORY_LABEL_BG_COLOR);
      DrawRectangleLines(textX - padding, textY - padding, textWidth + (padding * 2), 16, INVENTORY_LABEL_BORDER_COLOR);
      
      DrawText(categoryName, textX, textY, INVENTORY_LABEL_FONT_SIZE, BLACK);
    }
  }

  EndScissorMode();
}

void UnloadPantry() {
  // Textures are now managed globally, no need to unload
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
