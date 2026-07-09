#include "engine.hpp"
#include "raylib-cpp.hpp"
#include "scenes.hpp"
#include "time.hpp"
#include <iostream>

int main() {

      const int windowWidth = 1280;
      const int windowHeight = 720;

      raylib::Window window(windowWidth, windowHeight, "monke cooks");

      if (ballers::bigBanana == false) {
            std::cout << "\n\nbig banana\n\n" << std::endl;
      }

      // 1 time loads
      SceneManager sceneManager;

      // loadable scenes
      Scene scenes[] = {
            Scene("MainMenu", LoadMainMenu, UpdateMainMenu, DrawMainMenu,
                  UnloadMainMenu),
            Scene("Game", LoadGame, UpdateGame, DrawGame, UnloadGame),
      };

      sceneManager.add(scenes[0]); // MainMenu
      sceneManager.add(scenes[1]); // Game

      while (!window.ShouldClose()) {
            // variables per frame
            Time::Update();

            // updating
            sceneManager.update();

            // drawing
            BeginDrawing();

            ::ClearBackground(raylib::Color::RayWhite());
            ::ClearBackground(raylib::Color::RayWhite());
            sceneManager.draw();

            EndDrawing();
      }

      return 0;
}
