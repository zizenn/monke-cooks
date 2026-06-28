#include "raylib-cpp.hpp"
#include "scenes.hpp"
#include <iostream>

int main() {

  ::SetTraceLogLevel(LOG_NONE);
  raylib::Window window(800, 600, "monke cooks");

  if (bigBanana == false) {
    std::cout << "\n\nbig banana\n\n" << std::endl;
  }

  // 1 time loads
  SceneManager sceneManager;
  Scene scenes[] = {mainMenu("mainMenu", loadMainMenu, updateMainMenu,
                             drawMainMenu, unloadMainMenu)};

  while (!window.ShouldClose()) {
    // variables per frame
    float deltaTime = ::GetFrameTime();

    // updating

    // drawing
    BeginDrawing();

    ::ClearBackground(raylib::Color::RayWhite());

    EndDrawing();
  }

  return 0;
}
