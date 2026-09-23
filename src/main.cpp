#include <iostream>

#include "Color.hpp"
#include "RenderTexture.hpp"
#include "Window.hpp"
#include "keymaps.hpp"
#include "level.hpp"
#include "raylib.h"
#include "scenes.hpp"
#include "time.hpp"

int main() {
      // set window width + height
      const int windowWidth = 1280;
      const int windowHeight = 720;

      ::SetExitKey(KEY_NULL);  // disable exit key (default is ESC)

      raylib::Window window(windowWidth, windowHeight, "monke cooks");
      SetTargetFPS(200);

      std::cout << "\n\n\n";
      std::cout << "game logs:\n";

      // 1 time loads
      scene::SceneManager sceneMan;
      lvl::Level lvlMan;
      lvlMan.Initialize();

      // variables definitions
      raylib::RenderTexture2D renTex =
            LoadRenderTexture(windowWidth, windowHeight);

      // load the first scene
      sceneMan.ChangeScene(scene::Scenes::MainMenu);

      while (!window.ShouldClose()) {
            // variables per frame
            Time::Update();
            keymaps::Keymaps::Update();
            sceneMan.CheckSceneChange();

            // updating
            sceneMan.Update();

            // drawing
            renTex.BeginMode();  // put ALL drawing logic between these modes

            ::ClearBackground(raylib::Color::RayWhite());
            sceneMan.Draw();

            renTex.EndMode();  // put ALL drawing logic between these modes

            BeginDrawing();
            ClearBackground(BLACK);  // Clear the actual screen buffer

            // CRITICAL: OpenGL texture coordinates are upside-down.
            // We pass a negative height to flip it right-side up.
            Rectangle sourceRec = {0.0f, 0.0f, (float)renTex.texture.width,
                                   -(float)renTex.texture.height};
            Rectangle destRec = {0.0f, 0.0f, (float)windowWidth,
                                 (float)windowHeight};
            Vector2 origin = {0.0f, 0.0f};

            // Draw the texture covering the whole window
            DrawTexturePro(renTex.texture, sourceRec, destRec, origin, 0.0f,
                           WHITE);

            DrawFPS(10, 10);
            EndDrawing();
      }

      std::cout << "game logs end:";
      std::cout << "\n\n\n";

      return 0;
}
