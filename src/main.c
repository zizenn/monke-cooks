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
  SCENE_FULL_SCREEN,  // Bottom layer: Replaces previous full-screen scene
  SCENE_OVERLAY,      // Top layer: Stacks on top of everything
  SCENE_BACKGROUND    // Middle layer: Drawn after full-screen, before overlays
} SceneType;

// === SCENE HANDLER STRUCT ===
typedef struct {
  SCREENS id;
  SceneType type;
  void (*init)(void);
  void (*update)(void);
  void (*draw)(void);
  void (*unload)(void);
} SceneHandler;

// === SCENE MANAGER STRUCT ===
typedef struct {
  SCREENS *scenes;
  int count;
  int capacity;
  SCREENS backgroundScene;  // Background scene - drawn after fullscreen, before overlays
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
  { RECIPE_SCREEN, SCENE_BACKGROUND, InitRecipeBook, UpdateRecipeBook, DrawRecipeBook, UnloadRecipeBook },
};

#define SCENE_HANDLER_COUNT (sizeof(sceneHandlers) / sizeof(sceneHandlers[0]))

// === GLOBALS ===
SCREENS currentScreen = MAIN_MENU;
bool shouldQuit = false;
RenderTexture2D canvas;
int targetFPS = 60;

// === FORWARD DECLARATIONS ===
void PushScene(SCREENS scene);
void PopScene(void);
void PopBackgroundScene(void);
void HandleScene(SCREENS scene, SceneAction action);
void CleanupSceneManager(void);
bool IsOverlayActive(void);

// === SCENE MANAGER FUNCTIONS ===

// Find scene type
static SceneType GetSceneType(SCREENS scene) {
  for (int i = 0; i < SCENE_HANDLER_COUNT; i++) {
    if (sceneHandlers[i].id == scene) {
      return sceneHandlers[i].type;
    }
  }
  return SCENE_FULL_SCREEN;  // Default to full-screen
}

bool IsOverlayActive(void) {
  if (sceneManager.count < 2) return false;  // Need at least one full-screen + one overlay
  SCREENS topScene = sceneManager.scenes[sceneManager.count - 1];
  return GetSceneType(topScene) == SCENE_OVERLAY;
}

bool IsBackgroundSceneActive(void) {
  return sceneManager.backgroundScene != 0;
}

void InitSceneManager(void) {
  sceneManager.capacity = 10;
  sceneManager.scenes = (SCREENS *)malloc(10 * sizeof(SCREENS));
  sceneManager.count = 0;
  PushScene(MAIN_MENU);
}

void PushScene(SCREENS scene) {
  SceneType type = GetSceneType(scene);
  
  // If pushing a background scene, store it separately
  if (type == SCENE_BACKGROUND) {
    // Unload previous background scene if it exists
    if (sceneManager.backgroundScene != 0) {
      HandleScene(sceneManager.backgroundScene, SCENE_UNLOAD);
    }
    sceneManager.backgroundScene = scene;
    HandleScene(scene, SCENE_INIT);
    return;
  }
  
  // If pushing a full-screen scene, pop the current one first
  if (type == SCENE_FULL_SCREEN && sceneManager.count > 0) {
    PopScene();
  }
  
  // Grow array if needed
  if (sceneManager.count >= sceneManager.capacity) {
    sceneManager.capacity *= 2;
    sceneManager.scenes = (SCREENS *)realloc(sceneManager.scenes, 
      sceneManager.capacity * sizeof(SCREENS));
  }
  
  // Add scene to stack
  sceneManager.scenes[sceneManager.count++] = scene;
  currentScreen = scene;  // Update global for compatibility
  
  // Initialize the scene
  HandleScene(scene, SCENE_INIT);
}

void PopScene(void) {
  if (sceneManager.count == 0) return;
  
  SCREENS scene = sceneManager.scenes[sceneManager.count - 1];
  sceneManager.count--;
  
  // Unload the scene
  HandleScene(scene, SCENE_UNLOAD);
  
  // Update global to top scene (or background if only background left)
  if (sceneManager.count > 0) {
    currentScreen = sceneManager.scenes[sceneManager.count - 1];
  } else if (sceneManager.backgroundScene != 0) {
    currentScreen = sceneManager.backgroundScene;
  }
}

void PopBackgroundScene(void) {
  if (sceneManager.backgroundScene == 0) return;
  
  SCREENS scene = sceneManager.backgroundScene;
  sceneManager.backgroundScene = 0;
  
  // Unload the scene
  HandleScene(scene, SCENE_UNLOAD);
  
  // Update global to top scene if it exists
  if (sceneManager.count > 0) {
    currentScreen = sceneManager.scenes[sceneManager.count - 1];
  }
}

// Find and execute scene action
void HandleScene(SCREENS scene, SceneAction action) {
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
  if (sceneManager.backgroundScene != 0) {
    PopBackgroundScene();
  }
  free(sceneManager.scenes);
}

int main() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(1280, 720, "monke cooks");
  SetExitKey(KEY_NULL);
  InitAudioDevice();

  if (!LoadTextureManifest("assets/data/textures.json")) {
    CloseAudioDevice();
    CloseWindow();
    return 1;
  }

  if (!LoadItemData("assets/data/ingredients.json", "assets/data/stocked_fridge.json", "assets/data/stocked_pantry.json")) {
    UnloadAllTextures();
    CloseAudioDevice();
    CloseWindow();
    return 1;
  }
  
  // Start async texture loading and music manager
  StartTextureLoader();
  StartMusicManager();
  
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
    
    // Update notifications timer
    UpdateNotifications();
    
    // 1. UPDATE fullscreen scenes
    for (int i = 0; i < sceneManager.count; i++) {
      if (GetSceneType(sceneManager.scenes[i]) == SCENE_FULL_SCREEN) {
        HandleScene(sceneManager.scenes[i], SCENE_UPDATE);
      }
    }
    
    // 2. UPDATE background scene (middle layer)
    if (sceneManager.backgroundScene != 0) {
      HandleScene(sceneManager.backgroundScene, SCENE_UPDATE);
    }
    
    // 3. UPDATE overlay scenes (top layer)
    for (int i = 0; i < sceneManager.count; i++) {
      if (GetSceneType(sceneManager.scenes[i]) == SCENE_OVERLAY) {
        HandleScene(sceneManager.scenes[i], SCENE_UPDATE);
      }
    }
    
    BeginTextureMode(canvas);
    ClearBackground(BLACK);
    
    // 1. DRAW fullscreen scenes
    for (int i = 0; i < sceneManager.count; i++) {
      if (GetSceneType(sceneManager.scenes[i]) == SCENE_FULL_SCREEN) {
        HandleScene(sceneManager.scenes[i], SCENE_DRAW);
      }
    }
    
    // 2. DRAW background scene (middle layer)
    if (sceneManager.backgroundScene != 0) {
      HandleScene(sceneManager.backgroundScene, SCENE_DRAW);
    }

    // 3. DRAW overlay scenes (top layer)
    for (int i = 0; i < sceneManager.count; i++) {
      if (GetSceneType(sceneManager.scenes[i]) == SCENE_OVERLAY) {
        HandleScene(sceneManager.scenes[i], SCENE_DRAW);
      }
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
  UnloadItemData();
  UnloadAllTextures();
  UnloadRenderTexture(canvas);
  CloseAudioDevice();
  CloseWindow();

  return 0;
}
