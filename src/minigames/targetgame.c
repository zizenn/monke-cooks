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
#include "minigames/targetgame.h"

TargetGameResult targetGameResult = TARGET_GAME_RUNNING;

const int CENTER_X = 640;
const int CENTER_Y = 360;
const float RADIUS = 200.0f;
const float SPEED = 150.0f;
const int TOTAL_TARGETS = 3;
const float HIT_TOLERANCE = 10.0f;

static float playerRadius = RADIUS;
static float targetRadius = 0.0f;
static int targetsHit = 0;
static int targetsMissed = 0;
static bool waitingForNext = false;
static float waitTimer = 0.0f;
static bool hitDetected = false;
static bool hitSuccess = false;

static void SpawnCircle(void) {
    playerRadius = RADIUS;
    targetRadius = (float)GetRandomValue(20,70);
    hitDetected = false;
    hitSuccess = false;
}

void initTargetMinigame() {
    targetGameResult = TARGET_GAME_RUNNING;
    targetsHit = 0;
    targetsMissed = 0;
    waitTimer = 0.0f;
    SpawnCircle();
}

void UpdateTargetMinigame() {
    // if (targetGameResult != TARGET_GAME_RUNNING) return;
    float deltaTime = GetFrameTime();

    if (targetRadius <= 0.0f) {
        targetsMissed++;
    }

    waitTimer -= deltaTime;
    if (waitTimer <= 0.0f) {
        
    }

}