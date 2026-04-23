#include "external/raylib.h"
#include "external/raygui.h"
#include "game/cooking.h"
#include "game/screens.h"
#include "game/display_screen.h"
#include "game/globals.h"
#include "game/game.h"
#include "game/items.h"
#include "game/texture_cache.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"

// enums
typedef enum {
  WALKABLE, // this is 0
  COUNTER, // this is 1
  FRIDGE, // this is 2
  STOVE_STATION, // this is 3
  OVEN_STATION, // 4
  DEEP_FRY_STATION, // 5
  GRILL_STATION, // 6
  ASSEMBLY, // 7
  SINK, // 8
  CUTTING_STATION, // 9
  TRASH, // 10
  PANTRY, // 11
  GRINDING_STATION // 12
} TILE_TYPE;

// function prototypes
static void MovePlayer(int DIR);
static void Interact(void);
static int MenuNavigation(Rectangle *rects, int count, int *selected);
static void LoadMap(const char *filePath);
static void DrawTile(TILE_TYPE tile, int tx, int ty);
static void BuildStaticMapLayer(void);
void QuantityLower(whereIsItemFrom type);

// variables'
static bool isMenuOpen = false;
static MENU_TYPE currentMenu = NONE;
static RenderTexture2D staticMapLayer = {0};
static bool hasStaticMapLayer = false;

// tile-based movement
static TILE_TYPE *map = NULL;
static float moveSpeed = 1.0f;
static int currentTileX;
static int currentTileY;
static bool isMoving = false;
static bool spaceWasPressed = false;

// player
static Vector2 playerPos;
static DIRECTION facing;

// dialog_menus
static int selected = 0;

void InitGame(void) {
  currentTileX = 4;
  currentTileY = 4;
  playerPos = (Vector2){ TileToPixels(currentTileX), TileToPixels(currentTileY) };
  selected = 0;
  facing = DOWN;
  holding = (ItemType){ -1, -1, 0 };
  currentPrepType = PREP_NONE;
  currentMenu = NONE;
  isMoving = false;
  isMenuOpen = false;
  itemFrom = FROM_NONE;

  // mapfile
  LoadMap("assets/maps/map1.txt");
  BuildStaticMapLayer();

  // textures
  playerTexture[0] = LoadTexture("assets/monkey/imgs/up.png");
  playerTexture[1] = LoadTexture("assets/monkey/imgs/down.png");
  playerTexture[2] = LoadTexture("assets/monkey/imgs/left.png");
  playerTexture[3] = LoadTexture("assets/monkey/imgs/right.png");
}

void UpdateGame(void) {
  if (!isMenuOpen) {
    if (IsKeyPressed(KEY_W) && !isMoving) {
      isMoving = true;
      facing = UP;
      MovePlayer(UP);
      isMoving = false;
    }
    if (IsKeyPressed(KEY_S) && !isMoving) {
      isMoving = true;
      facing = DOWN;
      MovePlayer(DOWN);
      isMoving = false;
    }
    if (IsKeyPressed(KEY_A) && !isMoving) {
      isMoving = true;
      facing = LEFT;
      MovePlayer(LEFT);
      isMoving = false;
    }
    if (IsKeyPressed(KEY_D) && !isMoving) {
      isMoving = true;
      facing = RIGHT;
      MovePlayer(RIGHT);
      isMoving = false;
    }
  }

  // interact
  if (IsKeyPressed(KEY_SPACE) && !isMenuOpen) {
    Interact();
    spaceWasPressed = true;
  }

  // track when space is released
  if (IsKeyUp(KEY_SPACE)) {
    spaceWasPressed = false;
  }
}

void DrawGame(void) {
  ClearBackground(WHITE);

  if (hasStaticMapLayer) {
    DrawTextureRec(
      staticMapLayer.texture,
      (Rectangle){0, 0, (float)staticMapLayer.texture.width, -(float)staticMapLayer.texture.height},
      (Vector2){0, 0},
      WHITE
    );
  } else if (map != NULL) {
    for (int row = 0; row < mapRows; row++) {
      for (int col = 0; col < mapCols; col++) {
        TILE_TYPE tile = map[row * mapCols + col];
        DrawTile(tile, TileToPixels(col), TileToPixels(row));
      }
    }
  } 

  Texture2D playerSprite = playerTexture[facing];
  Rectangle playerSource = { 0.0f, 0.0f, (float)playerSprite.width, (float)playerSprite.height };
  Rectangle playerDest = { TilesToPixels(currentTileX), TilesToPixels(currentTileY), (float)TILE_SIZE, (float)TILE_SIZE };
  DrawTexturePro(playerSprite, playerSource, playerDest, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);

  // dialog_menus
  int selection = 0;
  switch (currentMenu) {
    case NONE:
      break;

    case FRIDGE_MENU:
      isMenuOpen = false;
      currentMenu = NONE;
      currentScreen = FRIDGE_SCREEN;
      break;

    case PANTRY_MENU:
      isMenuOpen = false;
      currentMenu = NONE;
      currentScreen = PANTRY_SCREEN;
      break;

    case STOVE_MENU: 
      isMenuOpen = false;
      currentMenu = NONE;
      if (holding.cookType == PAN) {
        currentCookType = PAN;
        currentScreen = STOVE_SCREEN;
      }
      break;

    case OVEN_MENU: 
      isMenuOpen = false;
      currentMenu = NONE;
      if (holding.cookType == OVEN) {
        currentCookType = OVEN;
        currentScreen = OVEN_SCREEN;
      }
      break;

    case DEEP_FRY_MENU: 
      isMenuOpen = false;
      currentMenu = NONE;
       if (holding.cookType == DEEP_FRY) {
        currentCookType = DEEP_FRY;
        currentScreen = DEEP_FRY_SCREEN;
      }
      break;

    case GRILL_MENU: 
      isMenuOpen = false;
      currentMenu = NONE;
      if (holding.cookType == GRILL) {
        currentCookType = GRILL;
        currentScreen = GRILL_SCREEN;
      }
      break;
  }
}

void UnloadGame(void) {
  ReleaseTextureArray(playerTexture, 4);

  if (hasStaticMapLayer) {
    UnloadRenderTexture(staticMapLayer);
    staticMapLayer = (RenderTexture2D){0};
    hasStaticMapLayer = false;
  }

  if (map != NULL) {
    free(map);
    map = NULL;
  }
}

static void LoadMap(const char *filePath) {
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

  fclose(mapFile);
}

static void DrawTile(TILE_TYPE tile, int tx, int ty) {
  switch (tile) {
    case WALKABLE:
      DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, RAYWHITE);
      break;
    case COUNTER:
      DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, LIGHTGRAY);
      DrawText("counter", tx+4, ty+4, 14, WHITE);
      break;
    case FRIDGE:
      DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, DARKBLUE);
      DrawText("fridge", tx+4, ty+4, 14, WHITE);
      break;
    case STOVE_STATION:
      DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, GREEN);
      DrawText("stove", tx+4, ty+4, 14, BLACK);
      break;
    case OVEN_STATION:
      DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, YELLOW);
      DrawText("oven", tx+4, ty+4, 14, BLACK);
      break;
    case DEEP_FRY_STATION:
      DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, ORANGE);
      DrawText("deep fry", tx+4, ty+4, 14, BLACK);
      break;
    case GRILL_STATION:
      DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, RED);
      DrawText("grill", tx+4, ty+4, 14, BLACK);
      break;
    case ASSEMBLY:
      DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, GRAY);
      DrawText("assembly", tx+4, ty+4, 14, WHITE);
      break;
    case SINK:
      DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, BLUE);
      DrawText("sink", tx+4, ty+4, 14, BLACK);
      break;
    case CUTTING_STATION:
      DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, DARKPURPLE);
      DrawText("cutting", tx+4, ty+4, 14, WHITE);
      break;
    case TRASH:
      DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, BLACK);
      DrawText("trash", tx+4, ty+4, 14, WHITE);
      break;
    case PANTRY:
      DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, BROWN);
      DrawText("pantry", tx+4, ty+4, 14, WHITE);
      break;
    case GRINDING_STATION:
      DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, MAROON);
      DrawText("grind", tx+4, ty+4, 14, WHITE);
      break;
  }

  DrawRectangle(tx, ty, TILE_SIZE, 1, GRAY);
  DrawRectangle(tx, ty, 1, TILE_SIZE, GRAY);
}

static void BuildStaticMapLayer(void) {
  if (map == NULL) return;

  if (hasStaticMapLayer) {
    UnloadRenderTexture(staticMapLayer);
    staticMapLayer = (RenderTexture2D){0};
    hasStaticMapLayer = false;
  }

  staticMapLayer = LoadRenderTexture(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
  if (staticMapLayer.texture.id == 0) {
    TraceLog(LOG_WARNING, "Failed to create static map render texture");
    return;
  }

  BeginTextureMode(staticMapLayer);
  ClearBackground(WHITE);
  for (int row = 0; row < mapRows; row++) {
    for (int col = 0; col < mapCols; col++) {
      TILE_TYPE tile = map[row * mapCols + col];
      DrawTile(tile, TileToPixels(col), TileToPixels(row));
    }
  }
  EndTextureMode();
  hasStaticMapLayer = true;
}

static void MovePlayer(int DIR) {
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

static int MenuNavigation(Rectangle *rects, int count, int *selected) {
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

  if (IsKeyPressed(KEY_ESCAPE)) {
    isMenuOpen = false;
    currentMenu = NONE;
    return -1;
  }

  if (IsKeyPressed(KEY_SPACE) && !spaceWasPressed) return *selected + 1;
  return -1;
}

void QuantityLower(whereIsItemFrom type) {
  if (holding.categoryId < 0) return; // player holds nothing

  switch (type) {
    case FROM_FRIDGE:
      for (int i = 0; i < 11; i++) { // stockedFridge item count
        if (stockedFridge[i].categoryId == holding.categoryId) {
          if (stockedFridge[i].quantity > 0) stockedFridge[i].quantity--;
          break;
        }
      }
      break;

    case FROM_PANTRY:
      for (int i = 0; i < 10; i++) { // stockedPantry item count
        if (stockedPantry[i].categoryId == holding.categoryId) {
          if (stockedPantry[i].quantity > 0) stockedPantry[i].quantity--;
          break;
        }
      }
      break;

    default:
      break;
  }
}

static void Interact(void) {
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
    case PANTRY:
      currentMenu = PANTRY_MENU;
      break;
    case STOVE_STATION:
      currentMenu = STOVE_MENU;
      break;
    case OVEN_STATION:
      currentMenu = OVEN_MENU;
      break;
    case DEEP_FRY_STATION:
      currentMenu = DEEP_FRY_MENU;
      break;
    case GRILL_STATION:
      currentMenu = GRILL_MENU;
      break;
    case TRASH:
      if (holding.categoryId >= 0) {
        ReleaseTextureArray(playerTexture, 4);
        playerTexture[0] = LoadTexture("assets/monkey/imgs/up.png");
        playerTexture[1] = LoadTexture("assets/monkey/imgs/down.png");
        playerTexture[2] = LoadTexture("assets/monkey/imgs/left.png");
        playerTexture[3] = LoadTexture("assets/monkey/imgs/right.png");
        holding = (ItemType){ -1, -1, 0 };
        currentPrepType = PREP_NONE;
        itemFrom = FROM_NONE;
      }
      break;
    case ASSEMBLY:
      if (holding.categoryId >= 0) {
        PrepFood();
      }
      break;   
}
}
