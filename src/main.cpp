#include "Color.hpp"
#include "Window.hpp"
#include "keymaps.hpp"
#include "raylib.h"
#include "scenes.hpp"
#include "time.hpp"
#include <iostream>

int main() {

      // set window width + height
      const int windowWidth = 1280;
      const int windowHeight = 720;

      ::SetExitKey(KEY_NULL); // disable exit key (default is ESC)

      raylib::Window window(windowWidth, windowHeight, "monke cooks");

      std::cout << "\n\n\n";
      std::cout << "game logs:\n";

      // 1 time loads
      scene::SceneManager sceneManager;

      // load the first scene
      sceneManager.ChangeScene(scene::Scenes::MainMenu);

      while (!window.ShouldClose()) {
            // variables per frame
            Time::Update();
            keymaps::Keymaps::Update();
            sceneManager.CheckSceneChange();

            // updating
            sceneManager.Update();

            // drawing
            BeginDrawing();

            ::ClearBackground(raylib::Color::RayWhite());
            sceneManager.Draw();

            EndDrawing();
      }

      std::cout << "game logs end:";
      std::cout << "\n\n\n";

      return 0;
}
