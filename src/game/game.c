#include "core/enums.h"
#include "external/raylib.h"
#include "external/raygui.h"
#include "game/globals.h"
#include "game/game.h"
#include "game/texture_manager.h"
#include "game/thread_manager.h"
#include "game/display_screen.h"
#include "game/config.h"
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
static void CalculateKeyPress(void);
static void MovePlayer(DIRECTION DIR);
static void Interact(void);
static void LoadMap(const char *filePath);
static void DrawTile(TILE_TYPE tile, int tx, int ty);
static void BuildStaticMapLayer(void);
static int TileToPixels(int tiles);

// variables
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
  holding = (Holding){ -1, -1, COOK_NONE, FROM_FRIDGE, ARRAY_FOOD };
  currentPrepType = PREP_NONE;
  currentMenu = NONE;
  isMoving = false;
  isMenuOpen = false;

  // Wait for async texture loading to finish before starting game
  WaitForTextureLoader();

  // Initialize texture mappings
  InitTextureMap();

  // mapfile
  LoadMap("assets/maps/map1.txt");
  BuildStaticMapLayer();

  // Set player textures from texture manager
  playerTexture[0] = GetTexture(PLAYER_UP);
  playerTexture[1] = GetTexture(PLAYER_DOWN);
  playerTexture[2] = GetTexture(PLAYER_LEFT);
  playerTexture[3] = GetTexture(PLAYER_RIGHT);
}

void UpdateGame(void) {
  CalculateKeyPress();
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
    for (int row = 0; row < MAP_ROWS; row++) {
      for (int col = 0; col < MAP_COLS; col++) {
        TILE_TYPE tile = map[row * MAP_COLS + col];
        DrawTile(tile, TileToPixels(col), TileToPixels(row));
      }
    }
  } 

  Texture2D playerSprite = playerTexture[facing];
  
  // If holding an item, display the item texture instead
  if (holding.categoryId >= 0) {
    playerSprite = GetHeldItemTexture();
  }
  
  Rectangle playerSource = { 0.0f, 0.0f, (float)playerSprite.width, (float)playerSprite.height };
  Rectangle playerDest = { (float)TileToPixels(currentTileX), (float)TileToPixels(currentTileY), (float)TILE_SIZE, (float)TILE_SIZE };
  DrawTexturePro(playerSprite, playerSource, playerDest, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);

  // dialog_menus
  int selection = 0;
  switch (currentMenu) {
    case NONE:
      break;

    case INVENTORY_MENU:
      isMenuOpen = false;
      currentMenu = NONE;
      currentScreen = INVENTORY_SCREEN;
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
  // Textures are managed by texture_manager and cleaned up in main()
  FreeTextureMap();

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

  map = malloc(MAP_COLS * MAP_ROWS * sizeof(TILE_TYPE));

  int value;
  for (int y = 0; y < MAP_ROWS; y++) {
    for (int x = 0; x < MAP_COLS; x++) {
      fscanf(mapFile, "%d", &value);
      map[y * MAP_COLS + x] = (TILE_TYPE)value;
    }
  }

  fclose(mapFile);
}

static void DrawTile(TILE_TYPE tile, int tx, int ty) {
  switch (tile) {
    case WALKABLE:
      DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, TILE_WALKABLE_COLOR);
      break;
    case COUNTER:
      DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, TILE_COUNTER_COLOR);
      DrawText("counter", tx+TILE_LABEL_OFFSET_X, ty+TILE_LABEL_OFFSET_Y, TILE_LABEL_FONT_SIZE, TILE_COUNTER_TEXT_COLOR);
      break;
    case FRIDGE:
      DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, TILE_FRIDGE_COLOR);
      DrawText("fridge", tx+TILE_LABEL_OFFSET_X, ty+TILE_LABEL_OFFSET_Y, TILE_LABEL_FONT_SIZE, TILE_FRIDGE_TEXT_COLOR);
      break;
    case STOVE_STATION:
      DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, TILE_STOVE_COLOR);
      DrawText("stove", tx+TILE_LABEL_OFFSET_X, ty+TILE_LABEL_OFFSET_Y, TILE_LABEL_FONT_SIZE, TILE_STOVE_TEXT_COLOR);
      break;
    case OVEN_STATION:
      DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, TILE_OVEN_COLOR);
      DrawText("oven", tx+TILE_LABEL_OFFSET_X, ty+TILE_LABEL_OFFSET_Y, TILE_LABEL_FONT_SIZE, TILE_OVEN_TEXT_COLOR);
      break;
    case DEEP_FRY_STATION:
      DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, TILE_DEEP_FRY_COLOR);
      DrawText("deep fry", tx+TILE_LABEL_OFFSET_X, ty+TILE_LABEL_OFFSET_Y, TILE_LABEL_FONT_SIZE, TILE_DEEP_FRY_TEXT_COLOR);
      break;
    case GRILL_STATION:
      DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, TILE_GRILL_COLOR);
      DrawText("grill", tx+TILE_LABEL_OFFSET_X, ty+TILE_LABEL_OFFSET_Y, TILE_LABEL_FONT_SIZE, TILE_GRILL_TEXT_COLOR);
      break;
    case ASSEMBLY:
      DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, TILE_ASSEMBLY_COLOR);
      DrawText("assembly", tx+TILE_LABEL_OFFSET_X, ty+TILE_LABEL_OFFSET_Y, TILE_LABEL_FONT_SIZE, TILE_ASSEMBLY_TEXT_COLOR);
      break;
    case SINK:
      DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, TILE_SINK_COLOR);
      DrawText("sink", tx+TILE_LABEL_OFFSET_X, ty+TILE_LABEL_OFFSET_Y, TILE_LABEL_FONT_SIZE, TILE_SINK_TEXT_COLOR);
      break;
    case CUTTING_STATION:
      DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, TILE_CUTTING_STATION_COLOR);
      DrawText("cutting", tx+TILE_LABEL_OFFSET_X, ty+TILE_LABEL_OFFSET_Y, TILE_LABEL_FONT_SIZE, TILE_CUTTING_STATION_TEXT_COLOR);
      break;
    case TRASH:
      DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, TILE_TRASH_COLOR);
      DrawText("trash", tx+TILE_LABEL_OFFSET_X, ty+TILE_LABEL_OFFSET_Y, TILE_LABEL_FONT_SIZE, TILE_TRASH_TEXT_COLOR);
      break;
    case PANTRY:
      DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, TILE_PANTRY_COLOR);
      DrawText("pantry", tx+TILE_LABEL_OFFSET_X, ty+TILE_LABEL_OFFSET_Y, TILE_LABEL_FONT_SIZE, TILE_PANTRY_TEXT_COLOR);
      break;
    case GRINDING_STATION:
      DrawRectangle(tx, ty, TILE_SIZE, TILE_SIZE, TILE_GRINDING_STATION_COLOR);
      DrawText("grind", tx+TILE_LABEL_OFFSET_X, ty+TILE_LABEL_OFFSET_Y, TILE_LABEL_FONT_SIZE, TILE_GRINDING_STATION_TEXT_COLOR);
      break;
  }

  DrawRectangle(tx, ty, TILE_SIZE, TILE_GRID_LINE_WIDTH, TILE_GRID_COLOR);
  DrawRectangle(tx, ty, TILE_GRID_LINE_WIDTH, TILE_SIZE, TILE_GRID_COLOR);
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
  for (int row = 0; row < MAP_ROWS; row++) {
    for (int col = 0; col < MAP_COLS; col++) {
      TILE_TYPE tile = map[row * MAP_COLS + col];
      DrawTile(tile, TileToPixels(col), TileToPixels(row));
    }
  }
  EndTextureMode();
  hasStaticMapLayer = true;
}

static void CalculateKeyPress(void) {
  KeyboardKey key = GetKeyPressed();
  switch (key) {
    default:
      break;
      
    case KEY_W:
      if (!isMenuOpen) MovePlayer(UP);
      break;

    case KEY_A:
      if (!isMenuOpen) MovePlayer(LEFT);
      break;

    case KEY_S:
      if (!isMenuOpen) MovePlayer(DOWN);
      break;

    case KEY_D:
      if (!isMenuOpen) MovePlayer(RIGHT);
      break;

    case KEY_SPACE:
      Interact();
      break;
  }
}

static void MovePlayer(DIRECTION DIR) {
  isMoving = true;
  int nextTileX = currentTileX;
  int nextTileY = currentTileY;
  switch (DIR) {
    case UP:
      facing = UP;
      nextTileY--;
      break;
    case DOWN:
      facing = DOWN;
      nextTileY++;
      break;
    case LEFT:
      facing = LEFT;
      nextTileX--;
      break;
    case RIGHT:
      facing = RIGHT;
      nextTileX++;
      break;
  }

  if (nextTileX >= 0 && nextTileX < MAP_COLS && nextTileY >= 0 && nextTileY < MAP_ROWS) {
    if (map[nextTileY * MAP_COLS + nextTileX] == WALKABLE) {
      currentTileX = nextTileX;
      currentTileY = nextTileY;
    }
  }
  isMoving = false;
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

  facingType = map[facingTileY * MAP_COLS + facingTileX];

  switch (facingType) {
    default:
      break;
    case FRIDGE:
      currentInventoryType = FROM_FRIDGE;
      currentMenu = INVENTORY_MENU;
      break;
    case PANTRY:
      currentInventoryType = FROM_PANTRY;
      currentMenu = INVENTORY_MENU;
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
        holding = (Holding){ -1, -1, COOK_NONE, FROM_FRIDGE, ARRAY_FOOD };
        currentPrepType = PREP_NONE;
      }
      break;
    case ASSEMBLY:
      if (holding.categoryId >= 0) {
        PrepFood(ASSEMBLY);
      }
      break;
    case SINK:
      if (holding.categoryId >= 0) {
        PrepFood(SINK);
      }
      break;
    case CUTTING_STATION:
      if (holding.categoryId >= 0) {
        PrepFood(CUTTING_STATION);
      }
      break;
    case GRINDING_STATION:
      if (holding.categoryId >= 0) {
        PrepFood(GRINDING_STATION);
      }
      break;
  }
}

static int TileToPixels(int tiles) {
  return tiles * TILE_SIZE;
}