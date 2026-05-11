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

typedef struct {
  Rectangle bounds;
  Color color;
} Box;

// variables
RECIPE_STATUS recipeStatus;
// rectangles
Box recipePanelBox = { {724, 0, 300, 576}, GRAY }; // full panel
Box *recipeItemBox = NULL;
int recipeItemBoxCount;

void InitRecipeBook() {
  recipeStatus = state.recipe_book.recipeStatus;
  recipeItemBoxCount = 2;
  recipeItemBox = malloc(recipeItemBoxCount * sizeof(Box));

  // for if there is an order
//  recipeItemBox[] = {
//    { {25, 25, 250, 40}, GRAY }, // recipe book ribbon
//    { {25, 25, 250, 40}, GRAY } // method box
//  };

}

void UpdateRecipeBook() {

}

void DrawRecipeBook() {
  GuiPanel(recipePanelBox.bounds, "recipe book");
//  for (int i = 0; i < sizeof(recipeItemBox) / sizeof(recipeItemBox[0]); i++) {
//    int remappedBoundsX = recipePanelBox.bounds.x + recipeItemBox[i].bounds.x;
//    int remappedBoundsY = recipePanelBox.bounds.y + recipeItemBox[i].bounds.y;
//    Rectangle remappedBounds = { remappedBoundsX, remappedBoundsY, recipeItemBox[i].bounds.width, recipeItemBox[i].bounds.height };
//    DrawRectangleRec(remappedBounds, recipeItemBox[i].color);
//  }

}

void UnloadRecipeBook() {

}
