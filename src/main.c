#include "game/display_screen.h"
#include "game/buttons.h"
#include "external/raylib.h"
#include "game/screens.h"
#include "game/globals.h"
#include "stdbool.h"

// globals
GameScreen currentScreen = MAIN_MENU;
bool shouldQuit = false;
RenderTexture2D canvas = {0};

// screens
GameScreen screensToNotUnloadFromGame[] = {
  FRIDGE_SCREEN,
  STOVE_SCREEN,
  OVEN_SCREEN,
  DEEP_FRY_SCREEN,
  GRILL_SCREEN
};

bool returnToGameAndUnload = false;

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
    case STOVE_SCREEN:
    case OVEN_SCREEN:
    case DEEP_FRY_SCREEN:
    case GRILL_SCREEN:
      InitCook();
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
    case BARMINIGAME_SCREEN:
      UnloadBarMinigame();
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
    case BARMINIGAME_SCREEN:
      UpdateBarMinigame();
      break;
    case STOVE_SCREEN:
    case OVEN_SCREEN:
    case DEEP_FRY_SCREEN:
    case GRILL_SCREEN:
      UpdateCook();
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
      DrawGame();
      DrawFridge();
      break;
    case BARMINIGAME_SCREEN:
      DrawBarMinigame();
      break;
    case STOVE_SCREEN:
      DrawGame();
      DrawCook();
      break;
    case OVEN_SCREEN:
      DrawGame();
      DrawCook();
      break;
    case DEEP_FRY_SCREEN:
      DrawGame();
      DrawCook();
      break;
    case GRILL_SCREEN:
      DrawGame();
      DrawCook();
      break;

    default:
      break;
  }
}

static void SwitchScene(GameScreen *activeScene, GameScreen nextScene) {
  GameScreen from = *activeScene;
  bool unloadGame = true;
 
  switch (from) {
    case FRIDGE_SCREEN:
      returnToGameAndUnload = true;
      break;
    case STOVE_SCREEN:
      returnToGameAndUnload = true;
      break;
    case OVEN_SCREEN:
      returnToGameAndUnload = true;
      break;
    case DEEP_FRY_SCREEN:
      returnToGameAndUnload = true;
      break;
    case GRILL_SCREEN:
      returnToGameAndUnload = true;
      break;

    default:
      returnToGameAndUnload = false;
      break;
  }

  for (int i = 0; i < sizeof(screensToNotUnloadFromGame) / sizeof(screensToNotUnloadFromGame[0]); i++) {
    if (nextScene == screensToNotUnloadFromGame[i]) {
      unloadGame = false;
    }
  }

  if (from == GAME && unloadGame == false) {
    EnterScene(nextScene);
  } else if (returnToGameAndUnload == true && nextScene == GAME) {
    ExitScene(from);
  } else {
    ExitScene(from);
    EnterScene(nextScene);
  }

  *activeScene = nextScene;
}

int main() {
  GameScreen activeScene = currentScreen;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(1280, 720, "monke cooks");
  SetExitKey(KEY_NULL);
  InitAudioDevice();
  SetTargetFPS(60);

  //temporary button setup figure this one out later
  ButtonStyles[BUTTON_STYLE_PRIMARY] = (ButtonStyle){
    .texture = LoadTexture("assets/ui/button_primary.png"),
    .hasTexture = true,
    .tint = WHITE,
    .textColor = (Color){255, 255, 104, 100},
    .textFocused = WHITE,
    .nineSlice = true,
    .borderTop = 5,
    .borderBottom = 4,
    .borderLeft = 4,
    .borderRight = 4,
  };
  ButtonStyles[BUTTON_STYLE_DANGER] = (ButtonStyle){
    .texture = LoadTexture("assets/ui/button_danger.png"),
    .hasTexture = true,
    .tint = WHITE,
    .textColor = (Color){255, 182, 216, 100},
    .textFocused = WHITE,
    .nineSlice = true,
    .borderTop = 5,
    .borderBottom = 4,
    .borderLeft = 4,
    .borderRight = 4,
  };


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
