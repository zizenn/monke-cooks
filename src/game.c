#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "screen_manager.h"
#include "globals.h"
#include "game.h"

typedef enum {
  LEFT,
  RIGHT,
  UP,
  DOWN
} DIRECTION;

typedef enum {
  WALKABLE,
  NON_WALKABLE,
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

static RenderTexture2D canvas;
static bool isMoving = false;
static bool isMenuOpen = false;
static bool spaceWasPressed = false;

// tiling system
static const int TILE_SIZE = 64;
static const int VISIBLE_ROWS = 9;
static const int VISIBLE_COLS = 16;
static const int VIRTUAL_WIDTH = 64*16;
static const int VIRTUAL_HEIGHT = 64*9;

static float moveSpeed = 1.0f;
static int currentTileX;
static int currentTileY;

// tile-based movement
static TILE_TYPE worldTile[][18] = {
    { NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE},
    { NON_WALKABLE, WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,    COUNTER,     COUNTER,     COUNTER,     COUNTER,     COUNTER,     COUNTER,     COUNTER,     COUNTER,    NON_WALKABLE},
    { NON_WALKABLE, WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,    COUNTER,     COUNTER,     COUNTER,     COUNTER,     COUNTER,     COUNTER,     COUNTER,     COUNTER,    NON_WALKABLE},
    { NON_WALKABLE, WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,   NON_WALKABLE},
    { NON_WALKABLE, WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,   NON_WALKABLE},
    { NON_WALKABLE, WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,   NON_WALKABLE},
    { NON_WALKABLE, WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,   NON_WALKABLE},
    { NON_WALKABLE, WALKABLE,     FRIDGE,       WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,   NON_WALKABLE},
    { NON_WALKABLE, WALKABLE,     STOVE,        WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,   NON_WALKABLE},
    { NON_WALKABLE, WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,   NON_WALKABLE},
    { NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE},
};

// player
static Vector2 playerPos;
static DIRECTION facing = DOWN;
static HELD_ITEM holding = EMPTY;

// dialog_menus
static MENU_TYPE currentMenu = NONE;
static int selected = 0;

// Function prototypes
static void movePlayer(int DIR);
static void interact(void);
static int menuNavigation(Rectangle *rects, int count, int *selected);

void InitGame(void) {
  currentTileX = 5;
  currentTileY = 5;
  playerPos = (Vector2){ currentTileX * TILE_SIZE, currentTileY * TILE_SIZE };
  canvas = LoadRenderTexture(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
  selected = 0;
  facing = DOWN;
  holding = EMPTY;
  currentMenu = NONE;
  isMoving = false;
  isMenuOpen = false;
  spaceWasPressed = false;
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

    int x = 0;
    int y = 0;

    for (int i = 1; i <= VISIBLE_ROWS; i++) {
      for (int j = 1; j <= VISIBLE_COLS; j++) {
        // top
        DrawRectangle(x, y, TILE_SIZE, 1, GRAY);
        // bottom
        DrawRectangle(x, y+TILE_SIZE, TILE_SIZE, 1, GRAY);
        // left
        DrawRectangle(x, y, 1, TILE_SIZE, GRAY);
        // right
        DrawRectangle(x+TILE_SIZE, y, 1, TILE_SIZE, GRAY);

        x += TILE_SIZE;
      }
      x = 0;
      y += TILE_SIZE;
    }

    // sidebar (place holder)
    DrawRectangle(TILE_SIZE*0, TILE_SIZE*2, TILE_SIZE*0.5, TILE_SIZE*1, BLACK);
    DrawText("sidebar", TILE_SIZE*0, TILE_SIZE*2, 26, GREEN);

    // counter (place holder)
    DrawRectangle(TILE_SIZE*8, TILE_SIZE*0, TILE_SIZE*8, TILE_SIZE*2, GRAY);
    DrawText("counter", TILE_SIZE*10, TILE_SIZE*0.5, 26, WHITE);

    // stove
    DrawRectangle(TILE_SIZE*1, TILE_SIZE*7, TILE_SIZE*1, TILE_SIZE*1, GREEN);
    DrawText("stove", TILE_SIZE*1, TILE_SIZE*7, 16, BLACK);

    // fridge
    DrawRectangle(TILE_SIZE*1, TILE_SIZE*6, TILE_SIZE*1, TILE_SIZE*1, BLUE);
    DrawText("fridge", TILE_SIZE*1, TILE_SIZE*6, 16, BLACK);

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

    DrawRectangle((currentTileX - 1)*TILE_SIZE, (currentTileY - 1)*TILE_SIZE, TILE_SIZE, TILE_SIZE, playerColor);     

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

  if (nextTileY >= 1 && nextTileY <= VISIBLE_ROWS && nextTileX >= 1 && nextTileX <= VISIBLE_COLS) {
    if (worldTile[nextTileY][nextTileX] == WALKABLE) {
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

  facingType = worldTile[facingTileY][facingTileX];

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
