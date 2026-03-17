#include "raylib.h"

const int VIRTUAL_WIDTH = 1280;
const int VIRTUAL_HEIGHT = 720;

RenderTexture2D canvas;

// globals
Vector2 mousePos = { 0 };
bool shouldQuit = false;

// player
Vector2 playerPos;

int main() {
  // start with VIRTUAL width initially
  int screenWidth = VIRTUAL_WIDTH;
  int screenHeight = VIRTUAL_HEIGHT;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(VIRTUAL_WIDTH, VIRTUAL_HEIGHT, "monke cooks");
  InitAudioDevice();
  SetTargetFPS(60);

  // the "virtual screen" init
  RenderTexture2D canvas = LoadRenderTexture(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

  // variables
  Texture2D monke_front = LoadTexture("assets/monke_front.png");
  Vector2 playerPos = (Vector2){ 640, 360 };

  while (!WindowShouldClose() && !shouldQuit) {
    // variable setting
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    mousePos = GetMousePosition();
    
    if (IsKeyPressed(KEY_W)) playerPos.y -= 25;
    if (IsKeyPressed(KEY_S)) playerPos.y += 25;
    if (IsKeyPressed(KEY_A)) playerPos.x -= 25;
    if (IsKeyPressed(KEY_D)) playerPos.x += 25;

    // all drawing goes in this PUT YOUR DRAWING IN THIS DO NOT PUT IN BEGIN DRAWING
    BeginTextureMode(canvas);
      ClearBackground(WHITE);

      // sidebar (template)
      DrawRectangle(0, 0, 300, 720, BLACK);
      DrawText("sidebar", 50, 360, 26, WHITE);

      // counter (template)
      DrawRectangle(300, 0, 980, 130, GRAY);
      DrawText("counter", 700, 30, 26, WHITE);

      DrawTextureEx(monke_front, playerPos, 0, 2, WHITE); // using DrawTextureEx() because it allows for scaling & rotation

    EndTextureMode();

    // stretching / scaling the canvas (VIRTUAL screen) onto the physical screen
    Rectangle source = {0, 0, canvas.texture.width, -canvas.texture.height }; // the minus symbol b4 the height is there bc raylib flips the y axis (starts from top instead of bottm)
    Rectangle dest = { 0, 0, GetScreenWidth(), GetScreenHeight() };
    Vector2 origin = { 0, 0 }; // says what the origin of the screen is (top-left corner)

    // this shouldnt be changed unless u really need to which wont happen (this basically draws the whole BeginTextureMode but scales it so it works)
    BeginDrawing();
      ClearBackground(WHITE);
      DrawTexturePro(canvas.texture, source, dest, origin, 0.0f, WHITE);
    EndDrawing();

  }

  // close

  // textures
  UnloadTexture(monke_front);

  // the stuff idk wat to call it
  CloseAudioDevice();
  CloseWindow();

  return 0;

}