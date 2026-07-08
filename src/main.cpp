#include "raylib-cpp.hpp"
#include "scenes.hpp"
#include "time.hpp"
#include <iostream>

int main() {

  ::SetTraceLogLevel(LOG_NONE);
  raylib::Window window(800, 600, "monke cooks");

  if (bigBanana == false) {
    std::cout << "\n\nbig banana\n\n" << std::endl;
  }

  // 1 time loads
  SceneManager sceneManager;
  while (!window.ShouldClose()) {
    // variables per frame
    Time::Update();

    // updating

    // drawing
    BeginDrawing();

    ::ClearBackground(raylib::Color::RayWhite());

    EndDrawing();
  }

  return 0;
}
