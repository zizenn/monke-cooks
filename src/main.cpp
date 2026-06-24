#include "raylib-cpp.hpp"

int main() {
  raylib::Window window(800, 600, "monke_cooks");
  SetTargetFPS(60);

  raylib::Texture2D texture("assets/monkey/imgs/down.png");

  while (!WindowShouldClose()) {
    BeginDrawing();
    window.ClearBackground(raylib::Color::RayWhite());
    DrawText("monke_cooks", 190, 200, 20, LIGHTGRAY);
    texture.Draw(100, 100, raylib::Color::RayWhite());
    EndDrawing();
  }

  return 0;
}
