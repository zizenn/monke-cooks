#include "game/display_screen.h"
#include "external/raylib.h"
#include "game/screens.h"
#include "game/globals.h"

// globals
GameScreen current = MAIN_MENU;
bool shouldQuit = false;

static void EnterScene(GameScreen scene) {
  switch (scene) {
    case MAIN_MENU:
      InitMainMenu();
      break;
    case CAMPAIGN_MENU:
      InitCampaignMenu();
      break;
    case GAME:
      InitGame();
      break;
    case FRIDGE_SCREEN:
      InitFridge();
      break;
    default:
      break;
  }
}

static void ExitScene(GameScreen scene) {
  switch (scene) {
    case MAIN_MENU:
      UnloadMainMenu();
      break;
    case CAMPAIGN_MENU:
      UnloadCampaignMenu();
      break;
    case GAME:
      UnloadGame();
      break;
    case FRIDGE_SCREEN:
      UnloadFridge();
      break;
    default:
      break;
  }
}

static void UpdateScene(GameScreen scene) {
  switch (scene) {
    case MAIN_MENU:
      UpdateMainMenu();
      break;
    case CAMPAIGN_MENU:
      UpdateCampaignMenu();
      break;
    case GAME:
      UpdateGame();
      break;
    case FRIDGE_SCREEN:
      UpdateFridge();
      break;
    default:
      break;
  }
}

static void DrawScene(GameScreen scene) {
  switch (scene) {
    case MAIN_MENU:
      DrawMainMenu();
      break;
    case CAMPAIGN_MENU:
      DrawCampaignMenu();
      break;
    case GAME:
      DrawGame();
      break;
    case FRIDGE_SCREEN:
      DrawFridge();
      break;
    default:
      break;
  }
}

int main() {
  GameScreen activeScene = current;

  InitWindow(1280, 720, "monke cooks");
  InitAudioDevice();
  SetTargetFPS(60);

  EnterScene(activeScene);

  while (!WindowShouldClose() && !shouldQuit) {
    if (current != activeScene) {
      ExitScene(activeScene);
      activeScene = current;
      EnterScene(activeScene);
    }

    UpdateScene(activeScene);

    if (current != activeScene) {
      ExitScene(activeScene);
      activeScene = current;
      EnterScene(activeScene);
    }

    BeginDrawing();
    DrawScene(activeScene);
    EndDrawing();
  }

  ExitScene(activeScene);
  CloseAudioDevice();
  CloseWindow();

  return 0;

}
