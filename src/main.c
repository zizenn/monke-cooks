#include "raylib.h"

// globals
Vector2 mousePos = { 0 };
bool shouldQuit = false;

void button(char hover_color[], char color[], char name[], char text[], Vector2 mousePos);

int main() {
  InitWindow(1280, 720, "monke cooks");
  InitAudioDevice();
  SetTargetFPS(60);


  while (!WindowShouldClose() && !shouldQuit) {
    // variable setting
    mousePos = GetMousePosition();

    // UpdateGame();

    BeginDrawing();

    // DrawGame();

    EndDrawing();

  }

  // close
  CloseAudioDevice();
  CloseWindow();

  return 0;

}