#include "include/screen_menu.h"
#include "raylib.h"
#include "include/screen_manager.h"
#include "include/globals.h"

// globals
Vector2 mousePos = { 0 };
GameScreen current = MAIN_MENU;

static inline bool hovered(Rectangle rec); //no longer includes additional parameter mousePos
static inline bool clicked(int mouseBtn, Rectangle rec); //no longer includes additional parameter mousePos
void button(char hover_color[], char color[], char name[], char text[], Vector2 mousePos);

int main() {
  InitWindow(1280, 720, "monke cooks");
  InitAudioDevice();
  SetTargetFPS(60);

  InitMainMenu();
  InitCampaignMenu();

  while (!WindowShouldClose()) {
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
      }

    EndDrawing();

  }

  // close
  UnloadMainMenu();
  UnloadCampaignMenu(); //optimise unloading so it doesn't RE-unload everything when more menus added
  CloseAudioDevice();
  CloseWindow();

  return 0;

}