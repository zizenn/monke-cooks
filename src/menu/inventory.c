#include "core/enums.h"
#include "core/types.h"
#include "external/raylib.h"
#include "external/raygui.h"
#include "game/display_screen.h"
#include "game/globals.h"
#include "game/game.h"
#include "game/items.h"
#include "game/texture_manager.h"
#include "game/config.h"
#include "game/state.h"
#include "stdio.h"
#include "ctype.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"

// textures
static Texture2D *currentTextures = NULL;

// vector2s
static Vector2 scrollOffset = {0, 0};

// variables
static bool confirm;
static char searchBarText[64] = "";
static bool searchEditMode = false;
static int *matchedItems = NULL;
static int matchCount = 0;
static char lastSearchText[INVENTORY_SEARCH_BOX_MAX_LENGTH] = "";
static int selectedItem;
static char (*lowerCategoryNames)[INVENTORY_CATEGORY_NAME_MAX_LENGTH] = NULL;
static StockItem *stockedArray = NULL;
static int ingredientCount;

static bool wasSpaceRemoved = false;

// sizing
static const int COLUMNS = INVENTORY_COLUMNS;
static const int ITEM_WIDTH = INVENTORY_ITEM_WIDTH;
static const int ITEM_HEIGHT = INVENTORY_ITEM_HEIGHT;

// function prototypes
static bool FuzzyFinder(const char* search, const char* name);
static void ToLowerString(const char* source, char* output, int outputSize);

void InitInventory() {
  // Set inventory type (FROM_FRIDGE or FROM_PANTRY)
  // This should be set by the caller before InitInventory() is called
  
  // variable setting
  confirm = false;
  searchBarText[0] = '\0';
  searchEditMode = true;
  selectedItem = holding.categoryId;
  
  // Consume key presses and character input from SPACE that opened inventory
  while (GetKeyPressed() != 0) {}
  while (GetCharPressed() != 0) {}
  wasSpaceRemoved = true;
  
  state.inventory.selectedMatchIndex = 0;
  state.inventory.mouseSelectionArmed = false;

  // Determine which inventory to use based on currentInventoryType
  switch (currentInventoryType) {
    case FROM_FRIDGE:
      stockedArray = stockedFridge;
      ingredientCount = stockedFridgeCount;
      break;

    case FROM_PANTRY:
      stockedArray = stockedPantry;
      ingredientCount = stockedPantryCount;
      break;

    default:
      return;  // Invalid inventory type
  }

  // Allocate dynamic arrays based on ingredientCount
  if (matchedItems != NULL) free(matchedItems);
  matchedItems = (int *)malloc(ingredientCount * sizeof(int));
  
  if (lowerCategoryNames != NULL) free(lowerCategoryNames);
  lowerCategoryNames = (char (*)[INVENTORY_CATEGORY_NAME_MAX_LENGTH])malloc(
    ingredientCount * INVENTORY_CATEGORY_NAME_MAX_LENGTH * sizeof(char)
  );
  
  if (currentTextures != NULL) free(currentTextures);
  currentTextures = (Texture2D *)malloc(ingredientCount * sizeof(Texture2D));
  
  if (ingredientCount > 0 &&
      (matchedItems == NULL || lowerCategoryNames == NULL || currentTextures == NULL)) {
    if (matchedItems != NULL) {
      free(matchedItems);
      matchedItems = NULL;
    }
    if (lowerCategoryNames != NULL) {
      free(lowerCategoryNames);
      lowerCategoryNames = NULL;
    }
    if (currentTextures != NULL) {
      free(currentTextures);
      currentTextures = NULL;
    }
    ingredientCount = 0;
    return;
  }

  // Load textures and prepare category names
  Holding savedHolding = holding;  // Save original
  for (int i = 0; i < ingredientCount; i++) {
    int categoryId = stockedArray[i].categoryId;
    
    // Get the appropriate category data based on inventory type
    FoodCategory *category = (currentInventoryType == FROM_FRIDGE) ? &allFridge[categoryId] : &allPantry[categoryId];
    
    // Load texture by creating a temporary holding struct for variant 0
    holding = (Holding){ categoryId, 0, category->variants[0].cook_type, currentInventoryType, ARRAY_FOOD };
    currentTextures[i] = GetHeldItemTexture();
    
    // Store lowercase category name for searching
    ToLowerString(category->categoryName, lowerCategoryNames[i], INVENTORY_CATEGORY_NAME_MAX_LENGTH);
  }
  holding = savedHolding;  // Restore original

  // Initialize the fuzzy finder filter
  matchCount = 0;
  for (int i = 0; i < ingredientCount; i++) {
    if (FuzzyFinder(searchBarText, lowerCategoryNames[i])) {
      matchedItems[matchCount++] = i;
    }
  }
  strcpy(lastSearchText, searchBarText);

  // Setup GUI styling
  GuiSetStyle(SCROLLBAR, SCROLLBAR_WIDTH, 3);
}

void UpdateInventory() {
  // Consume any remaining character input on first frame to block SPACE from entering text
  if (wasSpaceRemoved) {
    while (GetCharPressed() != 0) {}
    wasSpaceRemoved = false;
  }
  
  if (IsKeyPressed(KEY_ESCAPE)) {
    searchEditMode = false;
    PopScene();
  }
  
  // Handle search filter updates
  if (strcmp(searchBarText, lastSearchText) != 0) {
    matchCount = 0;

    for (int i = 0; i < ingredientCount; i++) {
      if (FuzzyFinder(searchBarText, lowerCategoryNames[i])) {
        matchedItems[matchCount++] = i;
      }
    }

    strcpy(lastSearchText, searchBarText);
    state.inventory.selectedMatchIndex = 0;  // Reset selection when search changes
    state.inventory.mouseSelectionArmed = false;
  }
  
  // Handle selection and confirmation
  if (matchCount > 0) {
    // Keyboard selection (grid movement with arrow keys)
    int layoutItemsPerRow = (panelBounds.width - 20) / ITEM_WIDTH;
    if (layoutItemsPerRow < 1) layoutItemsPerRow = 1;
    int totalRows = (matchCount + layoutItemsPerRow - 1) / layoutItemsPerRow;

    int selectedRow = state.inventory.selectedMatchIndex / layoutItemsPerRow;
    int selectedCol = state.inventory.selectedMatchIndex % layoutItemsPerRow;
    bool movedByKeyboard = false;

    if (IsKeyPressed(KEY_LEFT)) {
      if (selectedCol > 0) {
        selectedCol--;
        movedByKeyboard = true;
      }
    } else if (IsKeyPressed(KEY_RIGHT)) {
      int rightIndex = selectedRow * layoutItemsPerRow + (selectedCol + 1);
      if (rightIndex < matchCount) {
        selectedCol++;
        movedByKeyboard = true;
      }
    } else if (IsKeyPressed(KEY_UP)) {
      if (selectedRow > 0) {
        selectedRow--;
        movedByKeyboard = true;
      }
    } else if (IsKeyPressed(KEY_DOWN)) {
      if (selectedRow < totalRows - 1) {
        selectedRow++;
        movedByKeyboard = true;
      }
    }

    int newSelectedIndex = selectedRow * layoutItemsPerRow + selectedCol;
    if (newSelectedIndex >= matchCount) newSelectedIndex = matchCount - 1;
    state.inventory.selectedMatchIndex = newSelectedIndex;
    if (movedByKeyboard) state.inventory.mouseSelectionArmed = false;
    
    // Mouse click detection
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      Vector2 virtualMousePos = GetMousePosition();
      
      // Check if click is within inventory panel
      if (virtualMousePos.x >= panelBounds.x && virtualMousePos.x < panelBounds.x + panelBounds.width &&
          virtualMousePos.y >= panelBounds.y && virtualMousePos.y < panelBounds.y + panelBounds.height) {
        int startX = panelBounds.x + 5;
        int startY = panelBounds.y + 5;
        int maxWidth = panelBounds.width - 20;
        int itemsPerRow = maxWidth / ITEM_WIDTH;
        if (itemsPerRow < 1) itemsPerRow = 1;

        float localX = virtualMousePos.x - startX;
        float localY = virtualMousePos.y - startY + scrollOffset.y;
        int rowClicked = (int)(localY / ITEM_HEIGHT);
        int colClicked = (int)(localX / ITEM_WIDTH);
        int itemIndexClicked = rowClicked * itemsPerRow + colClicked;

        if (localX >= 0 && localY >= 0 &&
            colClicked >= 0 && colClicked < itemsPerRow &&
            itemIndexClicked >= 0 && itemIndexClicked < matchCount) {
          // Confirm only after an explicit prior mouse selection
          if (state.inventory.mouseSelectionArmed && itemIndexClicked == state.inventory.selectedMatchIndex) {
            // Confirm selection
            int selectedStockedIndex = matchedItems[state.inventory.selectedMatchIndex];
            int categoryId = stockedArray[selectedStockedIndex].categoryId;
            
            if (stockedArray[selectedStockedIndex].quantity > 0) {
              int variantId = 0;
              FoodCategory *category = (currentInventoryType == FROM_FRIDGE) ? &allFridge[categoryId] : &allPantry[categoryId];
              COOK_TYPE cookType = category->variants[variantId].cook_type;
              
              holding = (Holding){ categoryId, variantId, cookType, currentInventoryType, ARRAY_FOOD };
              
              if (currentInventoryType == FROM_PANTRY) {
                currentPrepType = category->variants[variantId].prep_type;
              }
              
              searchEditMode = false;
              PopScene();
            }
          } else {
            // First click selects and highlights the item
            state.inventory.selectedMatchIndex = itemIndexClicked;
            state.inventory.mouseSelectionArmed = true;
          }
        }
      }
    }
    
    // ENTER to confirm selection
    if (IsKeyPressed(KEY_ENTER)) {
      int selectedStockedIndex = matchedItems[state.inventory.selectedMatchIndex];
      int categoryId = stockedArray[selectedStockedIndex].categoryId;
      
      if (stockedArray[selectedStockedIndex].quantity > 0) {
        int variantId = 0;
        FoodCategory *category = (currentInventoryType == FROM_FRIDGE) ? &allFridge[categoryId] : &allPantry[categoryId];
        COOK_TYPE cookType = category->variants[variantId].cook_type;
        
        holding = (Holding){ categoryId, variantId, cookType, currentInventoryType, ARRAY_FOOD };
        
        if (currentInventoryType == FROM_PANTRY) {
          currentPrepType = category->variants[variantId].prep_type;
        }
        
        TraceLog(LOG_INFO, "category id: %d  |  variant id: %d  |  item name: %s", categoryId, variantId, category->variants->name);

        searchEditMode = false;
        PopScene();
      }
    }
  }
}

void DrawInventory() {
  int startX = panelBounds.x + 5;
  int startY = panelBounds.y + 5;
  int maxWidth = panelBounds.width - 20;
  int itemsPerRow = maxWidth / ITEM_WIDTH;
  if (itemsPerRow < 1) itemsPerRow = 1;
  int totalRows = (matchCount + itemsPerRow - 1) / itemsPerRow;

  Rectangle panelContent = {0, 0, panelBounds.width-15, totalRows * ITEM_HEIGHT};
  Rectangle searchBounds = {totalArea.x, totalArea.y+panelBounds.height, totalArea.width, 16};

  GuiScrollPanel(panelBounds, NULL, panelContent, &scrollOffset, NULL);
  if (GuiTextBox(searchBounds, searchBarText, INVENTORY_SEARCH_BOX_MAX_LENGTH, searchEditMode)) {
    searchEditMode = !searchEditMode;
    if (!searchEditMode && matchCount == 0 && searchBarText[0] != '\0') {
      PopScene();
    }
  }

  BeginScissorMode(panelBounds.x, panelBounds.y, panelBounds.width, panelBounds.height);
 
  if (matchCount != 0) {
    for (int i = 0; i < matchCount; i++) {
      int ingredientIdx = matchedItems[i];
      int row = i / itemsPerRow;
      int col = i % itemsPerRow;

      float xPos = startX + (col * ITEM_WIDTH);
      float yPos = startY + (row * ITEM_HEIGHT) - scrollOffset.y;

      // Highlight selected item behind texture
      if (i == state.inventory.selectedMatchIndex) {
        DrawRectangle(xPos, yPos, ITEM_WIDTH, ITEM_HEIGHT, Fade(BLUE, 0.45f));
      }

      // Draw quantity
      char quantityStr[10];
      snprintf(quantityStr, sizeof(quantityStr), "%d", stockedArray[ingredientIdx].quantity);
      DrawTextureEx(currentTextures[ingredientIdx], (Vector2){xPos, yPos}, 0, INVENTORY_TEXTURE_SCALE, WHITE);
      DrawRectangleRec((Rectangle){ xPos, yPos, INVENTORY_QUANTITY_BOX_WIDTH, INVENTORY_QUANTITY_BOX_HEIGHT }, LIGHTGRAY);
      DrawText(quantityStr, xPos+2, yPos+2, INVENTORY_QUANTITY_BOX_FONT_SIZE, BLACK);
      
      // Draw category label
      int categoryId = stockedArray[ingredientIdx].categoryId;
      FoodCategory *category = (currentInventoryType == FROM_FRIDGE) ? &allFridge[categoryId] : &allPantry[categoryId];
      const char* categoryName = category->categoryName;
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

void UnloadInventory() {
  // Free dynamically allocated arrays
  if (matchedItems != NULL) {
    free(matchedItems);
    matchedItems = NULL;
  }
  if (lowerCategoryNames != NULL) {
    free(lowerCategoryNames);
    lowerCategoryNames = NULL;
  }
  if (currentTextures != NULL) {
    free(currentTextures);
    currentTextures = NULL;
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
