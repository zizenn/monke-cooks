#include "external/raylib-cpp.hpp"

int main() {
  raylib::Window window(800, 600, "Hello Raylib-cpp");

  while (!window.ShouldClose()) {
    BeginDrawing();

    window.ClearBackground(raylib::Color::RayWhite());
    raylib::Text::Draw("Hello, Raylib-cpp!", 190, 200, 20,
                       raylib::Color::LightGray());

    EndDrawing();
  }

  return 0;
}
