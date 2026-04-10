#include "raylib.h"
#include "stdio.h"
#include "stdlib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "screen_menu.h"
#include "screen_manager.h"
#include "globals.h"

typedef enum {
  LEFT,
  RIGHT,
  UP,
  DOWN
} DIRECTION;

typedef enum {
  WALKABLE,
  COUNTER,
  STOVE,
  FRIDGE
} TILE_TYPE;

typedef enum {
  NONE,
  FRIDGE_MENU,
  STOVE_MENU
} MENU_TYPE;

typedef enum {
  EMPTY,
  RICE,
  MILK,
  EGG
} HELD_ITEM;

// Function prototypes
static void movePlayer(int DIR);
static void interact(void);
static int menuNavigation(Rectangle *rects, int count, int *selected);
static void loadMap(const char *filePath);


static RenderTexture2D canvas;
static bool isMoving = false;
static bool isMenuOpen = false;
static bool spaceWasPressed = false;

// tiling system
static const int TILE_SIZE = 64;
static const int VISIBLE_ROWS = 9;

static const int mapRows = 9;
static const int mapCols = 16;

static const int VIRTUAL_WIDTH = TILE_SIZE*16;
static const int VIRTUAL_HEIGHT = TILE_SIZE*9;

static float moveSpeed = 1.0f;
static int currentTileX;
static int currentTileY;

// tile-based movement
static TILE_TYPE *map = NULL;

// player
static Vector2 playerPos;
static DIRECTION facing = DOWN;
static HELD_ITEM holding = EMPTY;

// dialog_menus
static MENU_TYPE currentMenu = NONE;
static int selected = 0;

void InitGame(void) {
  currentTileX = 4;
  currentTileY = 4;
  playerPos = (Vector2){ currentTileX * TILE_SIZE, currentTileY * TILE_SIZE };
  canvas = LoadRenderTexture(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
  selected = 0;
  facing = DOWN;
  holding = EMPTY;
  currentMenu = NONE;
  isMoving = false;
  isMenuOpen = false;
  spaceWasPressed = false;
  loadMap("assets/maps/map1.txt");
}

void UpdateGame(void) {
  if (isMenuOpen) {
    // keep empty makes sure the player doesnt move while in a menu
    // also all menu logic is declared where the menu is declared
  }
  else {
    if (IsKeyPressed(KEY_W) && !isMoving) {
      isMoving = true;
      facing = UP;
      movePlayer(UP);
      isMoving = false;
    }
    if (IsKeyPressed(KEY_S) && !isMoving) {
      isMoving = true;
      facing = DOWN;
      movePlayer(DOWN);
      isMoving = false;
    }
    if (IsKeyPressed(KEY_A) && !isMoving) {
      isMoving = true;
      facing = LEFT;
      movePlayer(LEFT);
      isMoving = false;
    }
    if (IsKeyPressed(KEY_D) && !isMoving) {
      isMoving = true;
      facing = RIGHT;
      movePlayer(RIGHT);
      isMoving = false;
    }
  }

  // interact
  if (IsKeyPressed(KEY_SPACE) && !isMenuOpen) {
    interact();
    spaceWasPressed = true;
  }

  // track when space is released
  if (IsKeyUp(KEY_SPACE)) {
    spaceWasPressed = false;
  }
}

void DrawGame(void) {
  int screenWidth = GetScreenWidth();
  int screenHeight = GetScreenHeight();
  Color playerColor = BROWN;

  int dialogPosX = TILE_SIZE*5;
  int dialogPosY = TILE_SIZE*5;
  int dialogWidth = TILE_SIZE*5;
  int dialogHeight = TILE_SIZE*3;

  // all drawing goes in this PUT YOUR DRAWING IN THIS DO NOT PUT IN BEGIN DRAWING
  BeginTextureMode(canvas);
    ClearBackground(WHITE);

   // draw tiles from map
  for (int row = 0; row < mapRows; row++) {
    for (int col = 0; col < mapCols; col++) {
      TILE_TYPE tile = map[row * mapCols + col];
      int tx = col * TILE_SIZE;
      int ty = row * TILE_SIZE;

      switch (tile) {
        case WALKABLE:
          DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, RAYWHITE);
          break;
        case COUNTER:
          DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, GRAY);
          DrawText("counter", tx+4, ty+4, 16, WHITE);
          break;
        case STOVE:
          DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, GREEN);
          DrawText("stove", tx+4, ty+4, 16, BLACK);
          break;
        case FRIDGE:
          DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, BLUE);
          DrawText("fridge", tx+4, ty+4, 16, BLACK);
          break;
      }

      // grid lines
      DrawRectangle(tx, ty, TILE_SIZE, 1, GRAY);
      DrawRectangle(tx, ty, 1, TILE_SIZE, GRAY);
    }
  } 

    // player
    switch (holding) {
      case EMPTY:
        playerColor = BROWN;
        break;
      case MILK:
        playerColor = GREEN;
        break;
      case EGG:
        playerColor = BLUE;
        break;
      case RICE:
        playerColor = YELLOW;
        break;
    }

    DrawRectangle(currentTileX*TILE_SIZE, currentTileY*TILE_SIZE, TILE_SIZE, TILE_SIZE, playerColor);     

    // dialog_menus
    int selection = 0;
    switch (currentMenu) {
      case NONE:
        break;

      case FRIDGE_MENU:
        isMenuOpen = true;

        dialogPosX = TILE_SIZE*2+7;
        dialogPosY = TILE_SIZE*4+32;
        dialogWidth = TILE_SIZE*3-32;
        dialogHeight = TILE_SIZE*2-32;

        Rectangle fridgeRects[] = {
          { dialogPosX+TILE_SIZE/2, dialogPosY+TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2 }, // milk
          { dialogPosX+TILE_SIZE*1.5, dialogPosY+TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2 } // rice
        };

        // drawing
        DrawRectangle(dialogPosX, dialogPosY, dialogWidth, dialogHeight, BLACK);

        // menu nav b4 drawing buttons
        selection = menuNavigation(fridgeRects, 2, &selected);

        // --- handling logic ---

        // mouse input
        if (GuiButton(fridgeRects[0], "milk")) {
          holding = MILK;
          currentMenu = NONE;
          isMenuOpen = false;
          selected = 0;
        }

        if (GuiButton(fridgeRects[1], "rice")) {
          holding = RICE;
          currentMenu = NONE;
          isMenuOpen = false;
          selected = 0;
        }

        // keyboard input
        if (selection == 1) {
          holding = MILK;
          currentMenu = NONE;
          isMenuOpen = false;
          selected = 0;
        } else if (selection == 2) {
          holding = RICE;
          currentMenu = NONE;
          isMenuOpen = false;
          selected = 0;
        }

        break;

      case STOVE_MENU:
        isMenuOpen = true;

        dialogPosX = TILE_SIZE*2;
        dialogPosY = TILE_SIZE*5;
        dialogWidth = TILE_SIZE*3;
        dialogHeight = TILE_SIZE*2;

        Rectangle stoveRects[] = {
          { dialogPosX+TILE_SIZE/2, dialogPosY+TILE_SIZE, TILE_SIZE/2, TILE_SIZE/2 }, // milk
          { dialogPosX+TILE_SIZE*1.5, dialogPosY+TILE_SIZE, TILE_SIZE/2, TILE_SIZE/2 } // rice
        };

        // drawing
        DrawRectangle(dialogPosX, dialogPosY, dialogWidth, dialogHeight, BLACK);

        // menu nav b4 drawing buttons
        selection = menuNavigation(stoveRects, 2, &selected);

        // --- handling logic ---

        // mouse input
        if (GuiButton(stoveRects[0], "cook")) {
          currentMenu = NONE;
          isMenuOpen = false;
          selected = 0;
        }

        if (GuiButton(stoveRects[1], "monke")) {
          currentMenu = NONE;
          isMenuOpen = false;
          selected = 0;
        }

        // keyboard input
        if (selection == 1) {
          currentMenu = NONE;
          isMenuOpen = false;
          selected = 0;
        } else if (selection == 2) {
          currentMenu = NONE;
          isMenuOpen = false;
          selected = 0;
        }

        break;
    }

  EndTextureMode();

  // stretching / scaling the canvas (VIRTUAL screen) onto the physical screen
  Rectangle source = {0, 0, canvas.texture.width, -canvas.texture.height };
  Rectangle dest = { 0, 0, screenWidth, screenHeight };
  Vector2 origin = { 0, 0 };

  // this shouldnt be changed unless u really need to which wont happen (this basically draws the whole BeginTextureMode but scales it so it works)
  ClearBackground(WHITE);
  DrawTexturePro(canvas.texture, source, dest, origin, 0.0f, WHITE);
}

void UnloadGame(void) {
  UnloadRenderTexture(canvas);
  free(map);
}

static void loadMap(const char *filePath) {
  FILE *mapFile = fopen(filePath, "r");
  if (mapFile == NULL) {
    TraceLog(LOG_ERROR, "could not open map file %s", filePath);
    return;
  }

  map = malloc(mapCols * mapRows * sizeof(TILE_TYPE));

  int value;
  for (int y = 0; y < mapRows; y++) {
    for (int x = 0; x < mapCols; x++) {
      fscanf(mapFile, "%d", &value);
      map[y * mapCols + x] = (TILE_TYPE)value;
    }
  }
}

static void movePlayer(int DIR) {
  int nextTileX = currentTileX;
  int nextTileY = currentTileY;

  switch (DIR) {
    case UP:
      nextTileY--;
      break;
    case DOWN:
      nextTileY++;
      break;
    case LEFT:
      nextTileX--;
      break;
    case RIGHT:
      nextTileX++;
      break;
  }

  if (nextTileX >= 0 && nextTileX < mapCols && nextTileY >= 0 && nextTileY < mapRows) {
    if (map[nextTileY * mapCols + nextTileX] == WALKABLE) {
      currentTileX = nextTileX;
      currentTileY = nextTileY;
    }
  }
}

static int menuNavigation(Rectangle *rects, int count, int *selected) {
  if (IsKeyPressed(KEY_D)) (*selected)++;
  if (IsKeyPressed(KEY_A)) (*selected)--;
  if (*selected < 0) *selected = 0;
  if (*selected >= count) *selected = count - 1;

  // draw a rectangle behind to show selection (1 pixel larger on all sides)
  Rectangle selectionRect = {
    rects[*selected].x - 1,
    rects[*selected].y - 1,
    rects[*selected].width + 2,
    rects[*selected].height + 2
  };
  DrawRectangleRec(selectionRect, YELLOW);

  if (IsKeyPressed(KEY_C)) {
    isMenuOpen = false;
    currentMenu = NONE;
    return -1;
  }

  if (IsKeyPressed(KEY_SPACE) && !spaceWasPressed) return *selected + 1;
  return -1;
}

static void interact(void) {
  TILE_TYPE facingType;
  int facingTileX = currentTileX;
  int facingTileY = currentTileY;

  switch (facing) {
    default:
      break;
    case LEFT:
      facingTileX--;
      break;
    case RIGHT:
      facingTileX++;
      break;
    case UP:
      facingTileY--;
      break;
    case DOWN:
      facingTileY++;
      break;
  }

  facingType = map[facingTileY * mapCols + facingTileX];

  switch (facingType) {
    default:
      break;
    case FRIDGE:
      currentMenu = FRIDGE_MENU;
      break;
    case STOVE:
      currentMenu = STOVE_MENU;
      break;
  }
}
