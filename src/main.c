#include "screen_menu.h"
#include "raylib.h"
#include "screen_manager.h"
#include "globals.h"
#include "game.h"

// globals
Vector2 mousePos = { 0 };
GameScreen current = MAIN_MENU;
bool shouldQuit = false;

void button(char hover_color[], char color[], char name[], char text[], Vector2 mousePos);

int main() {
  InitWindow(1280, 720, "monke cooks");
  InitAudioDevice();
  SetTargetFPS(60);

  InitMainMenu();
  InitCampaignMenu();
  InitGame();

  while (!WindowShouldClose() && !shouldQuit) {
    // variable setting
    mousePos = GetMousePosition();

    switch (current) {
      case MAIN_MENU:
        UpdateMainMenu(); break;
      case SETTINGS_MENU:
        break;
      case CAMPAIGN_MENU:
        UpdateCampaignMenu(); break;
      case MULTIPLAYER_MENU:
        break;
      case ADVANCEMENTS_MENU:
        break;
      case GAME:
        UpdateGame(); break;
    }

    BeginDrawing();

      switch (current) {
        case MAIN_MENU:
          DrawMainMenu(); break;
        case SETTINGS_MENU:
          break;
        case CAMPAIGN_MENU:
          DrawCampaignMenu(); break;
        case MULTIPLAYER_MENU:
          break;
        case ADVANCEMENTS_MENU:
          break;
        case GAME:
          DrawGame(); break;
      }

    EndDrawing();

  }

  // close
  UnloadMainMenu();
  UnloadCampaignMenu(); //optimise unloading so it doesn't RE-unload everything when more menus added
  UnloadGame();
  CloseAudioDevice();
  CloseWindow();

  return 0;

}