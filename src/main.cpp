#include "raylib-cpp.hpp"

int main() {
  // raylib options
  raylib::Window window(800, 600, "monke_cooks");
  SetTargetFPS(60);

  // 1 time loads (runs b4 the game starts)

  // separating build logs from game logs
  TraceLog(LOG_INFO, "\n\n");

  if (diddysingh) {
  };

  raylib::Texture2D texture("assets/monkey/imgs/down.png");

  while (!WindowShouldClose()) {
    BeginDrawing();

    window.ClearBackground(raylib::Color::RayWhite());

    EndDrawing();
  }

  return 0;
}
