#include "core/scenes.h"
#include "core/globals.h"
#include "core/config.h"
#include "core/textures.h"
#include "game/keymap.h"
#include "game/level.h"
#include "stddef.h"

// defining variables
bool quitGame = false;
int gameFPS = 60;
float dt;
GAMESCENE baseCurrentScene = SCENE_GAME;
GAMESCENE currentScene = SCENE_GAME;
RenderTexture2D canvas;

// the function pointers
typedef void (*FuncPtr)(void);

// structs
typedef struct {
  GAMESCENE scene;
  FuncPtr loadFunction;
  FuncPtr updateFunction;
  FuncPtr drawFunction;
  FuncPtr unloadFunction;
} FuncMapping;

// enums
typedef enum {
  ACTION_LOAD,
  ACTION_UPDATE,
  ACTION_DRAW,
  ACTION_UNLOAD
} SCENEACTION;

// function prototypes
void HandleScene(GAMESCENE scene, SCENEACTION action);

// function mapping
FuncMapping functionMap[] = {
  {SCENE_GAME, LoadGame, UpdateGame, DrawGame, UnloadGame},
  {SCENE_MAIN, LoadMain, UpdateMain, DrawMain, UnloadMain}
};

// defining the sizes of these tables
#define MAP_SIZE (sizeof(functionMap) / sizeof(functionMap[0]))

int main() {
  InitWindow(initWidth, initHeight, "monke cooks");
  SetTargetFPS(gameFPS);

  LoadKeymap("playerData/keymap.json");
  LoadBaseTextureDatabase("gameData/baseTextureManifest.json");
  LoadLevelManifest("gameData/levels/levels.json");
  SwitchToLevelByIndex(1);

  canvas = LoadRenderTexture(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

  HandleScene(currentScene, ACTION_LOAD);

  while (!quitGame && !WindowShouldClose()) {
    // assigning per frame variables
    dt = GetFrameTime();
    float screenWidth = (float)GetScreenWidth();
    float screenHeight = (float)GetScreenHeight();

    if (baseCurrentScene != currentScene) {
      HandleScene(baseCurrentScene, ACTION_UNLOAD);
      baseCurrentScene = currentScene;
      HandleScene(currentScene, ACTION_LOAD);
    }

    HandleScene(currentScene, ACTION_UPDATE);

    // texture mode
    BeginTextureMode(canvas);

    ClearBackground(WHITE);
    HandleScene(currentScene, ACTION_DRAW);

    EndTextureMode();

    // draw the texture mode
    BeginDrawing();
    ClearBackground(WHITE);

    Rectangle source = {0.0f, 0.0f, (float)canvas.texture.width, -(float)canvas.texture.height };
    Rectangle dest = { 0.0f, 0.0f, screenWidth, screenHeight };
    Vector2 origin = { 0, 0 };

    DrawTexturePro(canvas.texture, source, dest, origin, 0.0f, WHITE);

    EndDrawing();
  }

  HandleScene(currentScene, ACTION_UNLOAD);
  UnloadKeymap();
  UnloadLevelManifest();
  UnloadBaseTextureDatabase();
  UnloadRenderTexture(canvas);
  CloseWindow();

  return 0;
}

void HandleScene(GAMESCENE scene, SCENEACTION action) {
  for (int i = 0; i < MAP_SIZE; i++) {
    if (functionMap[i].scene == scene) {
      FuncPtr targetFunc = NULL;
      switch (action) {
        case ACTION_LOAD: targetFunc = functionMap[i].loadFunction; break;
        case ACTION_UPDATE: targetFunc = functionMap[i].updateFunction; break;
        case ACTION_DRAW: targetFunc = functionMap[i].drawFunction; break;
        case ACTION_UNLOAD: targetFunc = functionMap[i].unloadFunction; break;
      }
      if (targetFunc != NULL) {
        targetFunc();
      }
      return;
    }
  }
}
