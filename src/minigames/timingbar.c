#include "external/raylib.h"
#include "external/raygui.h"
#include "game/screens.h"
#include "game/display_screen.h"
#include "game/globals.h"
#include "game/game.h"
#include "game/items.h"
#include "stdio.h"
#include "stdlib.h"
#include "game/timingbar.h"

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
const int WIN_TIME = 5.0f;

static float playerX;
static float safezoneX;
static float safezoneDir = 1.0f;
static float safeTime = 0.0f;
static bool movingRight = true;

void InitBarMinigame() {
    timingBarResult = TIMING_BAR_RUNNING;
    playerX = BAR_X;
    safezoneX = BAR_X + BAR_WIDTH / 2 - SAFEZONE_WIDTH / 2;
    safezoneDir = 1.0f;
    safeTime = 0.0f;
    movingRight = true;
}

void UpdateBarMinigame() {
    if (timingBarResult != TIMING_BAR_RUNNING) {
        return;
    }

    float DeltaTime = GetFrameTime();

    //inputs
    if (IsKeyDown(KEY_RIGHT)) {
        playerX += PLAYER_SPEED * DeltaTime;
    }
    if (IsKeyDown(KEY_LEFT)) {
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

    //count while indicator is in zone
    bool inZone = playerX >= safezoneX && playerX + PLAYER_WIDTH <= safezoneX + SAFEZONE_WIDTH;

    if (inZone) {
        safeTime += DeltaTime;
        if (safeTime >= WIN_TIME) {
            timingBarResult = TIMING_BAR_WIN;
            currentScreen = GAME;
        }
    } else {
        safeTime = 0.0f;
    }
}

void DrawBarMinigame() {
    ClearBackground(BLACK);
    //background
    DrawRectangle(BAR_X, BAR_Y, BAR_WIDTH, BAR_HEIGHT, DARKGRAY);
    
    //safe zone
    DrawRectangle((int)safezoneX, BAR_Y, SAFEZONE_WIDTH, BAR_HEIGHT, GREEN);

    //player indicator
    DrawRectangle((int)playerX, BAR_Y - 5, PLAYER_WIDTH, BAR_HEIGHT + 10, WHITE);

    //% progress
    DrawText(TextFormat("Hold: %.1f / %.1f", safeTime, WIN_TIME), BAR_X, BAR_Y - 30, 20, WHITE);
}

void UnloadBarMinigame() {

}
