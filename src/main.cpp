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
      // loadable scenes
      Scene scenes[] = {
            Scene("MainMenu", LoadMainMenu, UpdateMainMenu, DrawMainMenu,
                  UnloadMainMenu),
            Scene("Game", LoadGame, UpdateGame, DrawGame, UnloadGame),
      };

      SceneManager sceneManager;

      while (!window.ShouldClose()) {
            // variables per frame
            Time::Update();

            // updating
            sceneManager.addScene(scenes[0]); // MainMenu
            sceneManager.addScene(scenes[1]); // Game

            // drawing
            BeginDrawing();

            ::ClearBackground(raylib::Color::RayWhite());

            EndDrawing();
      }

      return 0;
}
