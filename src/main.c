#include "raylib.h"

// globals
Vector2 mousePos = { 0 };
bool shouldQuit = false;

// player
Vector2 playerPos;

void button(char hover_color[], char color[], char name[], char text[], Vector2 mousePos);

int main() {
  int windowWidth = 1280;
  int windowHeight = 720;

  InitWindow(windowWidth, windowHeight, "monke cooks");
  InitAudioDevice();
  SetTargetFPS(60);

  // variables
  Texture2D monke_front = LoadTexture("assets/monke_front.png");

  // player movement variables
  Vector2 playerPos = (Vector2){ 640, 360 };

  while (!WindowShouldClose() && !shouldQuit) {
    // variable setting
    mousePos = GetMousePosition();
    
    if (IsKeyPressed(KEY_W)) playerPos.y -= 25;
    if (IsKeyPressed(KEY_S)) playerPos.y += 25;
    if (IsKeyPressed(KEY_A)) playerPos.x -= 25;
    if (IsKeyPressed(KEY_D)) playerPos.x += 25;

    BeginDrawing();

      ClearBackground(WHITE);
      DrawTextureV(monke_front, playerPos, WHITE);

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