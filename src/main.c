#include "game/display_screen.h"
#include "external/raylib.h"
#include "game/screens.h"
#include "game/globals.h"

// globals
GameScreen currentScreen = MAIN_MENU;
bool shouldQuit = false;
RenderTexture2D canvas = {0};

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

static bool KeepGameLoadedBetweenScenes(GameScreen from, GameScreen to) {
  return (from == GAME && to == FRIDGE_SCREEN) || (from == FRIDGE_SCREEN && to == GAME);
}

static void SwitchScene(GameScreen *activeScene, GameScreen nextScene) {
  bool keepGameLoaded = KeepGameLoadedBetweenScenes(*activeScene, nextScene);

  if (!keepGameLoaded) {
    ExitScene(*activeScene);
  }

  *activeScene = nextScene;

  if (!keepGameLoaded) {
    EnterScene(*activeScene);
  }
}

int main() {
  GameScreen activeScene = currentScreen;

  InitWindow(1280, 720, "monke cooks");
  InitAudioDevice();
  SetTargetFPS(60);

  EnterScene(activeScene);

  while (!WindowShouldClose() && !shouldQuit) {
    if (currentScreen != activeScene) {
      SwitchScene(&activeScene, currentScreen);
    }

    UpdateScene(activeScene);

    if (currentScreen != activeScene) {
      SwitchScene(&activeScene, currentScreen);
    }

    BeginDrawing();
    DrawScene(activeScene);
    EndDrawing();
  }

  ExitScene(activeScene);
  if (activeScene == FRIDGE_SCREEN) {
    ExitScene(GAME);
  }
  CloseAudioDevice();
  CloseWindow();

  return 0;

}
