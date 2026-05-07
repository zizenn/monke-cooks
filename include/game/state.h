#ifndef STATE_H
#define STATE_H

#include "core/enums.h"
#include "core/types.h"
#include "external/raylib.h"

typedef enum {
  RUNNING,
  WIN,
  LOSE
} MINIGAME_STATUS;

typedef struct {
  // Player state
  struct {
    int tileX;
    int tileY;
    Vector2 pos;
    DIRECTION facing;
    bool isMoving;
  } player;
  
  // Inventory state
  struct {
    int selectedMatchIndex;
    bool mouseSelectionArmed;
  } inventory;
  
  // Prep state
  struct {
    int categoryId;
    Foods nextVariant;
    int nextVariantId;
  } prep;
  
  // Cooking state
  struct {
    Holding newHolding;
    int minigameSelection;
    char panelTitle[9];
    MINIGAME_STATUS minigameStatus;
    bool cookResultApplied;
  } cook;
  
  // Game state
  struct {
    bool isMenuOpen;
    MENU_TYPE currentMenu;
    int selected;
  } menu;
} GameState;

GameState* GetGameState(void);

#endif
