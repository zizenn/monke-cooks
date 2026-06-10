#include "core/textures.h"
#include "external/raylib.h"
#include "core/scenes.h"
#include "core/map.h"
#include "game/level.h"
#include "core/config.h"
#include "game/player.h"
#include "stddef.h"

void LoadGame(void) {
  // load level
  if (currentLevel != NULL && currentLevel->Load != NULL) currentLevel->Load(currentLevel);
}

void UpdateGame(void) {
  UpdateAsyncTextureLoading();

  if (!IsTextureDatabaseReady()) {
    return;
  }

  if (currentLevel != NULL && currentLevel->Update != NULL) {
    currentLevel->Update(currentLevel);
  }

  if (IsKeyPressed(KEY_N)) {
    SwitchToNextLevel();
  }

}

void DrawGame(void) {
  if (currentLevel != NULL && currentLevel->Draw != NULL) currentLevel->Draw(currentLevel);
}

void UnloadGame(void) {
  if (currentLevel != NULL && currentLevel->Unload != NULL) currentLevel->Unload(currentLevel);

}
