#include "Window.hpp"
#include "Color.hpp"
#include "scenes.hpp"
#include "time.hpp"

int main() {

      const int windowWidth = 1280;
      const int windowHeight = 720;

      raylib::Window window(windowWidth, windowHeight, "monke cooks");

      scene::SceneManager sceneManager;

      // load the first scene
      sceneManager.ChangeScene(scene::Scenes::MainMenu);

      while (!window.ShouldClose()) {
            // variables per frame
            Time::Update();
            sceneManager.CheckSceneChange();

            // updating
            sceneManager.Update();

            // drawing
            BeginDrawing();

            ::ClearBackground(raylib::Color::RayWhite());
            sceneManager.Draw();

            EndDrawing();
      }

      return 0;
}
