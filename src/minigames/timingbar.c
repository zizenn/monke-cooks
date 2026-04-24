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
#include "stdbool.h"
#include "math.h"
#include "minigames/timingbar.h"

TimingBarResult timingBarResult = TIMING_BAR_RUNNING;

//bar dimensions
float BAR_X = 200.0f;
float BAR_Y = 700.0f;
const int BAR_WIDTH = 400;
const int BAR_HEIGHT = 20;

//player
const int PLAYER_WIDTH = 10;
const float PLAYER_SPEED = 300.0f;
static bool playerFirstInput = true;

//safe zone
const float SAFEZONE_MAX = 200.0f;
const int SAFEZONE_WIDTH = 80;
static float safezoneX;
static float safezoneAccel = 5.0f;
static float safezoneVel = 150.0f;
static bool isSafe;

//win condition
const float WIN_TIME = 5.0f;
static float gameTime = 0.0f;

//lose condition
const float LOSE_TIME = 120.0f;

static float playerX;
static float safeTime = 0.0f;


// function prototypes
static bool changeDir();
static bool ChangeAccel();

void InitBarMinigame() {
    timingBarResult = TIMING_BAR_RUNNING;
    playerFirstInput = true;
    BAR_X = panelBounds.x + (panelBounds.width/2) - (BAR_WIDTH/2.0f);
    BAR_Y = panelBounds.y + panelBounds.height - BAR_HEIGHT - 35;
    playerX = BAR_X/2;
    safezoneX = BAR_X + BAR_WIDTH / 2.0f - SAFEZONE_WIDTH / 2.0f;
    safezoneAccel = 5.0f;
    safezoneVel = 0.0f;
    safeTime = 0.0f;
    gameTime = 0.0f;
}

bool UpdateBarMinigame() {
  if (gameTime >= LOSE_TIME) {
    timingBarResult = TIMING_BAR_LOSE;
  }
  if (timingBarResult == TIMING_BAR_LOSE) {
    SummonNotif("YOU LOSE!", ERROR);
    currentScreen = GAME;
    return false;
  }

  float DeltaTime = GetFrameTime();
  gameTime += DeltaTime;

  //inputs
  if (IsKeyDown(KEY_D)) {
    if (playerFirstInput) playerFirstInput = false;
    playerX += PLAYER_SPEED * DeltaTime;

  }
  if (IsKeyDown(KEY_A)) {
    if (playerFirstInput) playerFirstInput = false;
    playerX -= PLAYER_SPEED * DeltaTime;
  }

  //clamp player indicator to bar
  if (playerX < BAR_X) {
    playerX = BAR_X;
  }
  if (playerX > BAR_X + BAR_WIDTH - PLAYER_WIDTH) {
    playerX = BAR_X + BAR_WIDTH - PLAYER_WIDTH;
  }

  //bounce safearea off bar ends and increase acceleration after each gt
  safezoneVel += safezoneAccel;
  if (fabsf(safezoneVel) > SAFEZONE_MAX) {
    safezoneVel = SAFEZONE_MAX * (fabsf(safezoneVel)/safezoneVel);
  }

  safezoneX += safezoneVel * DeltaTime;
  if (safezoneX + SAFEZONE_WIDTH > BAR_X + BAR_WIDTH) {
    safezoneX = BAR_X + BAR_WIDTH - SAFEZONE_WIDTH;
    safezoneVel = fabsf(safezoneVel)*-1.0f;
    safezoneAccel = fabsf(safezoneAccel) * -1.0f;
  }
  if (safezoneX < BAR_X) {
    safezoneX = BAR_X;
    safezoneVel = fabsf(safezoneVel);
    safezoneAccel = fabsf(safezoneAccel);
  }

  if (ChangeAccel()) {
    if (safezoneAccel == 5.0f) {
      safezoneAccel = -5.0f;
    } else if (safezoneAccel == -5.0f) {
      safezoneAccel = 5.0f;
    }
  }

  //count while indicator is in zone
  isSafe = playerX >= safezoneX && playerX + PLAYER_WIDTH <= safezoneX + SAFEZONE_WIDTH;

  if (isSafe) {
    safeTime += DeltaTime;
    if (safeTime >= WIN_TIME) {
      timingBarResult = TIMING_BAR_WIN;
      if (itemFrom != FROM_NONE) {
        QuantityLower(itemFrom);
        itemFrom = FROM_NONE;
      }
      SummonNotif("YOU WIN!", SUCCESS);
      return true;
    }
  }
  else {
    if (safeTime >= 0) {
      safeTime -= DeltaTime*2.75;
      safezoneVel /= 1.3;
    }
  }
  return false;
}

void DrawBarMinigame() {
  //background
  DrawRectangle(BAR_X, BAR_Y, BAR_WIDTH, BAR_HEIGHT, DARKGRAY);
  
  //safe zone
  if (isSafe) {
    DrawRectangle((int)safezoneX, BAR_Y, SAFEZONE_WIDTH, BAR_HEIGHT, GREEN);
  } else {
    DrawRectangle((int)safezoneX, BAR_Y, SAFEZONE_WIDTH, BAR_HEIGHT, RED);
  }
  //player indicator
  DrawRectangle((int)playerX, BAR_Y - 5, PLAYER_WIDTH, BAR_HEIGHT + 10, BLACK);

  //% progress
  DrawText(TextFormat("Stay in the Zone! %.1f / %.1f", safeTime, WIN_TIME), BAR_X, BAR_Y - 30, 20, BLACK);

  //Time remaining
  DrawText(TextFormat("%.0fs Left", LOSE_TIME - gameTime), BAR_X + BAR_WIDTH - 100, BAR_Y - 30, 20, BLACK);

  //indicator before first input
  if (playerFirstInput) {
    DrawText(TextFormat("[D]"), BAR_X + 20, BAR_Y + (BAR_HEIGHT / 2.0f - 10), 20, BLACK);
    DrawText(TextFormat("[A]"), BAR_X + BAR_WIDTH - 40, BAR_Y + (BAR_HEIGHT / 2.0f - 10), 20, BLACK);
  }
}

void UnloadBarMinigame() {

}

static bool ChangeAccel() {
  int randomValue = GetRandomValue(1, 150);

  if (randomValue < 2) {
    return true;
  } else {
    return false;
  }
}
