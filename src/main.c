#include "core/enums.h"
#include "game/display_screen.h"
#include "game/buttons.h"
#include "game/items.h"
#include "game/texture_manager.h"
#include "game/thread_manager.h"
#include "game/globals.h"
#include "game/config.h"
#include "external/raylib.h"
#include "stdbool.h"
#include "stdlib.h"

// === SCENE ACTION ENUM ===
typedef enum {
  SCENE_INIT,
  SCENE_UPDATE,
  SCENE_DRAW,
  SCENE_UNLOAD
} SceneAction;

// === SCENE TYPE ENUM ===
typedef enum {
  SCENE_FULL_SCREEN,  // Replaces previous full-screen scene
  SCENE_OVERLAY       // Stacks on top
} SceneType;

// === SCENE HANDLER STRUCT ===
typedef struct {
  GameScreen id;
  SceneType type;
  void (*init)(void);
  void (*update)(void);
  void (*draw)(void);
  void (*unload)(void);
} SceneHandler;

// === SCENE MANAGER STRUCT ===
typedef struct {
  GameScreen *scenes;
  int count;
  int capacity;
} SceneManager;

static SceneManager sceneManager = {0};

// === SCENE HANDLERS TABLE ===
static const SceneHandler sceneHandlers[] = {
  { MAIN_MENU, SCENE_FULL_SCREEN, InitMainMenu, UpdateMainMenu, DrawMainMenu, UnloadMainMenu },
  { CAMPAIGN_MENU, SCENE_FULL_SCREEN, InitCampaignMenu, UpdateCampaignMenu, DrawCampaignMenu, UnloadCampaignMenu },
  { GAME_SCREEN, SCENE_FULL_SCREEN, InitGame, UpdateGame, DrawGame, UnloadGame },
  { INVENTORY_SCREEN, SCENE_OVERLAY, InitInventory, UpdateInventory, DrawInventory, UnloadInventory },
  { STOVE_SCREEN, SCENE_OVERLAY, InitCook, UpdateCook, DrawCook, UnloadCook },
  { OVEN_SCREEN, SCENE_OVERLAY, InitCook, UpdateCook, DrawCook, UnloadCook },
  { DEEP_FRY_SCREEN, SCENE_OVERLAY, InitCook, UpdateCook, DrawCook, UnloadCook },
  { GRILL_SCREEN, SCENE_OVERLAY, InitCook, UpdateCook, DrawCook, UnloadCook },
};

#define SCENE_HANDLER_COUNT (sizeof(sceneHandlers) / sizeof(sceneHandlers[0]))

// === GLOBALS ===
GameScreen currentScreen = MAIN_MENU;
bool shouldQuit = false;
RenderTexture2D canvas;
int targetFPS = 60;

// === FORWARD DECLARATIONS ===
void PushScene(GameScreen scene);
void PopScene(void);
void HandleScene(GameScreen scene, SceneAction action);
void CleanupSceneManager(void);

// === SCENE MANAGER FUNCTIONS ===

// Find scene type
static SceneType GetSceneType(GameScreen scene) {
  for (int i = 0; i < SCENE_HANDLER_COUNT; i++) {
    if (sceneHandlers[i].id == scene) {
      return sceneHandlers[i].type;
    }
  }
  return SCENE_FULL_SCREEN;  // Default to full-screen
}

void InitSceneManager(void) {
  sceneManager.capacity = 10;
  sceneManager.scenes = (GameScreen *)malloc(10 * sizeof(GameScreen));
  sceneManager.count = 0;
  PushScene(MAIN_MENU);
}

void PushScene(GameScreen scene) {
  // If pushing a full-screen scene, pop the current one first
  if (GetSceneType(scene) == SCENE_FULL_SCREEN && sceneManager.count > 0) {
    PopScene();
  }
  
  // Grow array if needed
  if (sceneManager.count >= sceneManager.capacity) {
    sceneManager.capacity *= 2;
    sceneManager.scenes = (GameScreen *)realloc(sceneManager.scenes, 
      sceneManager.capacity * sizeof(GameScreen));
  }
  
  // Add scene to stack
  sceneManager.scenes[sceneManager.count++] = scene;
  currentScreen = scene;  // Update global for compatibility
  
  // Initialize the scene
  HandleScene(scene, SCENE_INIT);
}

void PopScene(void) {
  if (sceneManager.count == 0) return;
  
  GameScreen scene = sceneManager.scenes[sceneManager.count - 1];
  sceneManager.count--;
  
  // Unload the scene
  HandleScene(scene, SCENE_UNLOAD);
  
  // Update global to top scene
  if (sceneManager.count > 0) {
    currentScreen = sceneManager.scenes[sceneManager.count - 1];
  }
}

// Find and execute scene action
void HandleScene(GameScreen scene, SceneAction action) {
  for (int i = 0; i < SCENE_HANDLER_COUNT; i++) {
    if (sceneHandlers[i].id == scene) {
      switch (action) {
        case SCENE_INIT:
          sceneHandlers[i].init();
          break;
        case SCENE_UPDATE:
          sceneHandlers[i].update();
          break;
        case SCENE_DRAW:
          sceneHandlers[i].draw();
          break;
        case SCENE_UNLOAD:
          sceneHandlers[i].unload();
          break;
      }
      return;
    }
  }
}

void CleanupSceneManager(void) {
  while (sceneManager.count > 0) {
    PopScene();
  }
  free(sceneManager.scenes);
}

int main() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(1280, 720, "monke cooks");
  SetExitKey(KEY_NULL);
  InitAudioDevice();
  
  // Start async texture loading and music manager
  StartTextureLoader();
  StartMusicManager();
  InitializeRuntimeCounts();
  
  SetTargetFPS(targetFPS);
  canvas = LoadRenderTexture(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

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

  // Initialize scene manager with MAIN_MENU as starting scene
  InitSceneManager();

  while (!WindowShouldClose() && !shouldQuit) {
    float screenWidth = (float)GetScreenWidth();
    float screenHeight = (float)GetScreenHeight();

    SetMouseOffset(0, 0);
    SetMouseScale((float)VIRTUAL_WIDTH / screenWidth, (float)VIRTUAL_HEIGHT / screenHeight);

    // Process texture loading on main thread (converts images to textures)
    ProcessTextureLoadingOnMainThread();
    
    // UPDATE all active scenes in the stack
    for (int i = 0; i < sceneManager.count; i++) {
      HandleScene(sceneManager.scenes[i], SCENE_UPDATE);
    }
    
    BeginTextureMode(canvas);
    ClearBackground(BLACK);

    // DRAW all visible scenes (bottom to top)
    for (int i = 0; i < sceneManager.count; i++) {
      HandleScene(sceneManager.scenes[i], SCENE_DRAW);
    }

    DrawNotifications();

    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);
    
    Rectangle source = {0.0f, 0.0f, (float)canvas.texture.width, -(float)canvas.texture.height };
    Rectangle dest = { 0.0f, 0.0f, screenWidth, screenHeight };
    Vector2 origin = { 0, 0 };

    DrawTexturePro(canvas.texture, source, dest, origin, 0.0f, WHITE);

    EndDrawing();
  }

  // Cleanup
  StopMusicManager();
  CleanupSceneManager();
  UnloadAllTextures();
  UnloadRenderTexture(canvas);
  CloseAudioDevice();
  CloseWindow();

  return 0;
}
