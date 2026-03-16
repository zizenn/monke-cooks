#include "include/screen_menu.h"
#include "raylib.h"
#include "include/screen_manager.h"
#include "include/globals.h"

// globals
Vector2 mousePos = { 0 };
GameScreen current = MAIN_MENU;

bool hovered(Vector2 mousePos, Rectangle rec);
bool clicked(int mouseBtn, Vector2 mousePos, Rectangle rec);
void button(char hover_color[], char color[], char name[], char text[], Vector2 mousePos);

int main() {
  InitWindow(1280, 720, "monke cooks");
  InitAudioDevice();
  SetTargetFPS(60);

  InitMenu();

  while (!WindowShouldClose()) {
    // variable setting
    mousePos = GetMousePosition();

    switch (current) {
      case MAIN_MENU:
        UpdateMenu(); break;
      case SETTINGS_MENU:
        break;
      case CAMPAIGN_MENU:
        break;
      case MULTIPLAYER_MENU:
        break;
      case ADVANCEMENTS_MENU:
        break;

    }

    BeginDrawing();

      switch (current) {
        case MAIN_MENU:
          DrawMenu(); break;
      }

    EndDrawing();

  }

  // close
  UnloadMenu();
  CloseAudioDevice();
  CloseWindow();

  return 0;

}