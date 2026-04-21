#include "external/raylib.h"
#include "external/raygui.h"
#include "game/screens.h"
#include "game/display_screen.h"
#include "game/globals.h"
#include "game/game.h"
#include "game/items.h"
#include "minigames/minigame.h"
#include "stdio.h"
#include "stdlib.h"
#include <stdbool.h>
#include "minigames/timingbar.h"

TimingBarResult timingBarResult = TIMING_BAR_RUNNING;

//bar dimensions
float BAR_X = 200.0f;
float BAR_Y = 260.0f;
const int BAR_WIDTH = 400;
const int BAR_HEIGHT = 40;

//player
const int PLAYER_WIDTH = 10;
const float PLAYER_SPEED = 300.0f;

//safe zone
const int SAFEZONE_WIDTH = 80;
const float SAFEZONE_SPEED = 150.0f;

//win condition
const float WIN_TIME = 5.0f;
static float gameTime = 0.0f;

static float playerX;
static float safezoneX;
static float safezoneDir = 1.0f;
static float safeTime = 0.0f;

// function prototypes
static bool changeDir();

void InitBarMinigame() {
    timingBarResult = TIMING_BAR_RUNNING;
    BAR_X = panelBounds.x + (panelBounds.width/2) - (BAR_WIDTH/2);
    BAR_Y = panelBounds.y + (panelBounds.height/2) - (BAR_HEIGHT/2);
    playerX = BAR_X/2;
    safezoneX = BAR_X + BAR_WIDTH / 2 - SAFEZONE_WIDTH / 2;
    safezoneDir = 1.0f;
    safeTime = 0.0f;
    gameTime = 0.0f;
}

void UpdateBarMinigame() {
  if (gameTime >= 120) {
    timingBarResult = TIMING_BAR_LOSE;
  }
  if (timingBarResult == TIMING_BAR_LOSE) {
    summonNotif("YOU LOSE!", ERROR);
    return;
  }

  float DeltaTime = GetFrameTime();
  gameTime += DeltaTime;

  //inputs
  if (IsKeyDown(KEY_D)) {
    playerX += PLAYER_SPEED * DeltaTime;
  }
  if (IsKeyDown(KEY_A)) {
    playerX -= PLAYER_SPEED * DeltaTime;
  }

  //clamp player indicator to bar
  if (playerX < BAR_X) {
    playerX = BAR_X;
  }
  if (playerX > BAR_X + BAR_WIDTH - PLAYER_WIDTH) {
    playerX = BAR_X + BAR_WIDTH - PLAYER_WIDTH;
  }

  //bounce safearea off bar ends
  safezoneX += SAFEZONE_SPEED * safezoneDir * DeltaTime;
  if (safezoneX + SAFEZONE_WIDTH > BAR_X + BAR_WIDTH) {
    safezoneX = BAR_X + BAR_WIDTH - SAFEZONE_WIDTH;
    safezoneDir = -1.0f;    
  }
  if (safezoneX < BAR_X) {
    safezoneX = BAR_X;
    safezoneDir = 1.0f;
  }

  if (changeDir()) {
    if (safezoneDir == 1.0f) {
      safezoneDir = -1.0f;
    } else if (safezoneDir == -1.0f) {
      safezoneDir = 1.0f;
    }
  }

  //count while indicator is in zone
  bool isSafe = playerX >= safezoneX && playerX + PLAYER_WIDTH <= safezoneX + SAFEZONE_WIDTH;

  if (isSafe) {
    safeTime += DeltaTime;
    if (safeTime >= WIN_TIME) {
      timingBarResult = TIMING_BAR_WIN;
      summonNotif("YOU WIN!", SUCCESS);
      currentScreen = GAME;
    }
  }
  else {
    if (safeTime >= 0) {
      safeTime -= DeltaTime*1.75;
    }
  }
}

void DrawBarMinigame() {
  //background
  DrawRectangle(BAR_X, BAR_Y, BAR_WIDTH, BAR_HEIGHT, DARKGRAY);
  
  //safe zone
  DrawRectangle((int)safezoneX, BAR_Y, SAFEZONE_WIDTH, BAR_HEIGHT, GREEN);

  //player indicator
  DrawRectangle((int)playerX, BAR_Y - 5, PLAYER_WIDTH, BAR_HEIGHT + 10, BLACK);

  //% progress
  DrawText(TextFormat("Hold: %.1f / %.1f", safeTime, WIN_TIME), BAR_X, BAR_Y - 30, 20, BLACK);
}

void UnloadBarMinigame() {

}

static bool changeDir() {
  int randomValue = GetRandomValue(1, 150);

  if (randomValue == 7) {
    return true;
  } else {
    return false;
  }
}
