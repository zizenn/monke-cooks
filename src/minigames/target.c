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
#include "minigames/target.h"

TargetGameResult targetGameResult = TARGET_GAME_RUNNING;

const int CENTER_X = 640;
const int CENTER_Y = 360;
const float OUTER_RADIUS = 200.0f;
const float SHRINK_SPEED = 80.0f;
const int MAX_ROUNDS = 3;
const float HIT_RANGE = 10.0f;
const int MIN_HITS = 3;

static float shrinkingRadius = OUTER_RADIUS;
static float targetRadius = 0.0f;
static int roundCount = 0;
static int hitCount = 0;
static bool waitingForNext = false;
static float waitTimer = 0.0f;
static bool justHit = false;
static bool hitSuccess = false;

static void SpawnTarget() {
    shrinkingRadius = OUTER_RADIUS;
    targetRadius = (float)GetRandomValue(20, OUTER_RADIUS-80);
    waitingForNext = false;
    justHit = false;
    hitSuccess = false;
}

void InitTargetMinigame() {
    targetGameResult = TARGET_GAME_RUNNING;
    hitCount = 0;
    roundCount = 0;
    waitTimer = 0.0f;
    SpawnTarget();
}

void UpdateTargetMinigame() {
    if (targetGameResult != TARGET_GAME_RUNNING) {
        return;
    }

    float DeltaTime = GetFrameTime();

    //wait between new targets
    if (waitingForNext) {
        waitTimer -= DeltaTime;
        if (waitTimer <= 0.0f) {
            roundCount++;
            if (roundCount >= MAX_ROUNDS) {
                //calculate amount missed if needed
                if (roundCount)
                //determine win/loss based on minimum hit requirement
                targetGameResult = roundCount<MIN_HITS ? TARGET_GAME_LOSE : TARGET_GAME_WIN;
            } else {
                SpawnTarget();
            }
        }
        return;
    }

    //decrease player circle size
    shrinkingRadius -= SHRINK_SPEED * DeltaTime;
    
    // if (shrinkingRadius <= 0.0f) {
    //    idk track misses here or activate ui change    
    // }

    //detect player input
    if (IsKeyPressed(KEY_SPACE)) {
        float difference = fabsf(shrinkingRadius - targetRadius);
        justHit = true;

        if (difference <= HIT_RANGE) {
            hitSuccess = true;
            waitingForNext = true;
            waitTimer = (float)GetRandomValue(5, 13)/10;
        }
        hitCount++;
    }


}