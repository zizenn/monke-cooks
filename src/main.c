#include "raylib.h"

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
  STATION
} TILE_TYPE;

RenderTexture2D canvas;
RenderTexture2D tileBorders;

// function prototypes
void movePlayer(int DIR);

// globals
Vector2 mousePos = { 0 };
bool shouldQuit = false;
bool isMoving = false;

// tiling system
const int TILE_SIZE = 64;

const int VIRTUAL_WIDTH = TILE_SIZE*16;
const int VIRTUAL_HEIGHT = TILE_SIZE*12;

float moveSpeed = 1.0f;
int currentTileX;
int currentTileY;

// tile-based movement
TILE_TYPE worldTile[][18] = {
    {NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE},
    {NON_WALKABLE, WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     COUNTER,     COUNTER,     COUNTER,     COUNTER,     COUNTER,     COUNTER,     COUNTER,     COUNTER,     WALKABLE, NON_WALKABLE},
    {NON_WALKABLE, WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     COUNTER,     COUNTER,     COUNTER,     COUNTER,     COUNTER,     COUNTER,     COUNTER,     COUNTER,     WALKABLE, NON_WALKABLE},
    {NON_WALKABLE, WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE, NON_WALKABLE},
    {NON_WALKABLE, WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE, NON_WALKABLE},
    {NON_WALKABLE, WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE, NON_WALKABLE},
    {NON_WALKABLE, WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE, NON_WALKABLE},
    {NON_WALKABLE, WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE, NON_WALKABLE},
    {NON_WALKABLE, WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE, NON_WALKABLE},
    {NON_WALKABLE, WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE, NON_WALKABLE},
    {NON_WALKABLE, WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE, NON_WALKABLE},
    {NON_WALKABLE, WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE, NON_WALKABLE},
    {NON_WALKABLE, WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,     WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE,    WALKABLE, NON_WALKABLE},
    {NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE, NON_WALKABLE},
};

int movementTile[][18] = { {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18},
                          {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18},
                          {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18},
                          {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18},
                          {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18},
                          {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18},
                          {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18},
                          {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18},
                          {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18},
                          {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18},
                          {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18},
                          {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18},
                          {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18},
                          {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18},};


// player
Vector2 playerPos;
Vector2 playerEyePos;

int main() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(VIRTUAL_WIDTH, VIRTUAL_HEIGHT, "monke cooks");
  InitAudioDevice();
  SetTargetFPS(60);
  
  // variables
  Texture2D monke_front = LoadTexture("assets/monke_front.png");
  currentTileX = 5;
  currentTileY = 5;
  playerPos = (Vector2){ currentTileX * TILE_SIZE, currentTileY * TILE_SIZE };
  //playerEyePos = (Vector2){ playerPos.x + 12, playerPos.y + 19 };
  canvas = LoadRenderTexture(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
  tileBorders = LoadRenderTexture(VIRTUAL_WIDTH, VIRTUAL_WIDTH);
  int tileX = 0;
  int tileY = 0;
  DIRECTION lastMoveDir = DOWN;

  while (!WindowShouldClose() && !shouldQuit) {
    // variable setting
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    Color playerColor = BROWN;

    mousePos = GetMousePosition();
    
    if (IsKeyPressed(KEY_W) && !isMoving) {
      isMoving = true;
      lastMoveDir = UP;
      movePlayer(UP);
      isMoving = false;
    }
    if (IsKeyPressed(KEY_S) && !isMoving) {
      isMoving = true;
      lastMoveDir = DOWN;
      movePlayer(DOWN);
      isMoving = false;
    }
    if (IsKeyPressed(KEY_A) && !isMoving) {
      isMoving = true;
      lastMoveDir = LEFT;
      movePlayer(LEFT);
      isMoving = false;
    }
    if (IsKeyPressed(KEY_D) && !isMoving) {
      isMoving = true;
      lastMoveDir = RIGHT;
      movePlayer(RIGHT);
      isMoving = false;
    }

    // all drawing goes in this PUT YOUR DRAWING IN THIS DO NOT PUT IN BEGIN DRAWING
    BeginTextureMode(canvas);
      ClearBackground(WHITE);
      //DrawTexture(tileBorders.texture, 0, 0, WHITE);

      int x = 0;
      int y = 0;

      for (int i = 0; i < 12 ; i++) {
        for (int j = 0; j < 16; j++) {
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
      DrawRectangle(TILE_SIZE*8, TILE_SIZE*1, TILE_SIZE*8, 128, GRAY);
      DrawText("counter", TILE_SIZE*10, TILE_SIZE*1.5, 26, WHITE);

      // player
      DrawRectangle(currentTileX*TILE_SIZE, currentTileY*TILE_SIZE, TILE_SIZE, TILE_SIZE, playerColor);

    EndTextureMode();

    // stretching / scaling the canvas (VIRTUAL screen) onto the physical screen
    Rectangle source = {0, 0, canvas.texture.width, -canvas.texture.height }; // the minus symbol b4 the height is there bc raylib flips the y axis (starts from top instead of bottm)
    Rectangle dest = { 0, 0, screenWidth, screenHeight };
    Vector2 origin = { 0, 0 }; // says what the origin of the screen is (top-left corner)

    // this shouldnt be changed unless u really need to which wont happen (this basically draws the whole BeginTextureMode but scales it so it works)
    BeginDrawing();
      ClearBackground(WHITE);
      DrawTexturePro(canvas.texture, source, dest, origin, 0.0f, WHITE);
    EndDrawing();

  }

  // --- close ---

  // textures
  UnloadRenderTexture(canvas);
  UnloadTexture(monke_front);

  // inits
  CloseAudioDevice();
  CloseWindow();

  return 0;

}

void movePlayer(int DIR) {
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

  if (worldTile[nextTileY][nextTileX] == WALKABLE) {
    currentTileX = nextTileX;
    currentTileY = nextTileY;
  }
}