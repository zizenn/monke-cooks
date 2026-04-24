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


const float OUTER_RADIUS = 100.0f;
const int SEGMENTS = 72;
#define TARGET_COUNT 10
const float SPEED = 120.0f;
const float HIT_TOLERANCE = 12.0f;
const float PLAYER_RADIUS = 40.0f;

typedef struct {
    float radius;
    bool active;
    bool hit;
} Target;

static int CenterX = 640;
static int CenterY = 360;

static Target targets[TARGET_COUNT];
static float playerRadius = PLAYER_RADIUS;
static int targetsMissed = 0;
static int targetsHit = 0;
static float missCooldown = 0;

static void SpawnTargets() {
    int targetPosition = 0;
    for (int i = 0; i < TARGET_COUNT; i++) {
        targetPosition += (float)GetRandomValue(20, 100);
        targets[i].radius = OUTER_RADIUS + (targetPosition);
        targets[i].active = true;
        targets[i].hit = false;
    }
}

void InitTargetMinigame() {
    targetGameResult = TARGET_GAME_RUNNING;
    targetsMissed = 0;
    targetsHit = 0;
    missCooldown = 0;
    playerRadius = PLAYER_RADIUS;
    CenterX = panelBounds.x + panelBounds.width/2;
    CenterY = panelBounds.y + panelBounds.height/2;
    SpawnTargets();
}

void UpdateTargetMinigame() {
    // if (targetGameResult != TARGET_GAME_RUNNING) return;

    float deltaTime = GetFrameTime();
    missCooldown -= deltaTime;

    bool anyActive = false;
    for (int i = 0; i < TARGET_COUNT; i++) {
        if (!targets[i].active) continue;
        anyActive = true;
        targets[i].radius -= SPEED * deltaTime;

        if (targets[i].radius <= 0.0f) {
            targets[i].active = false;
            targetsMissed++;
        }
    }

    if (IsKeyPressed(KEY_SPACE)) {
        bool hitAny = false;
        if (missCooldown <= 0.0f) {
            for (int i = 0; i < TARGET_COUNT; i++) {
                if (!targets[i].active) continue;
                float difference = fabsf(targets[i].radius - playerRadius);
                if (difference <= HIT_TOLERANCE) {
                    targets[i].active = false;
                    targets[i].hit = true;
                    hitAny = true;
                    break;
                }
            }
        }
        if (!hitAny) {
            missCooldown = 0.5f;
        }
    }

    //calculate targets hit separately to targets missed (targetsMissed as an accessory after passing center, targetsHit detected upon HIT)
    bool allInactive = true;
    targetsHit = 0;
    for (int i = 0; i < TARGET_COUNT; i++) {
        if (targets[i].hit) {
            targetsHit++;
        }
        if (targets[i].active) {
            allInactive = false;
        }
    }

    if (targetsHit == TARGET_COUNT && allInactive) {
        targetGameResult = TARGET_GAME_WIN;
        SummonNotif("YOU WIN!", SUCCESS);
    } else if (targetsHit != TARGET_COUNT && allInactive) {
        targetGameResult = TARGET_GAME_LOSE;
        SummonNotif("YOU LOSE!", ERROR);
    }
}

void DrawTargetMinigame() {
    DrawRing((Vector2){CenterX, CenterY}, OUTER_RADIUS, OUTER_RADIUS + 5.0f, 0, 360, SEGMENTS, GRAY);

    for (int i = 0; i < TARGET_COUNT; i++) {
        if (!targets[i].active||targets[i].radius >= OUTER_RADIUS) continue;
        DrawRing((Vector2){CenterX, CenterY}, targets[i].radius - 2.5f, targets[i].radius + 2.5f, 0, 360, SEGMENTS, GREEN);
    }

    if (missCooldown>0.0f) {
        DrawRing((Vector2){CenterX, CenterY}, playerRadius - 1.0f, playerRadius + 1.0f, 0, 360, SEGMENTS, RED);
    } else {
        DrawRing((Vector2){CenterX, CenterY}, playerRadius - 1.0f, playerRadius + 1.0f, 0, 360, SEGMENTS, BLACK);
    }
    //hit-miss counter
    DrawText(TextFormat("Targets Hit: %d, Missed: %d", targetsHit, targetsMissed), panelBounds.x + panelBounds.width / 2 - 70, panelBounds.y + panelBounds.height - 30, 20, BLACK);
}

void UnloadTargetMinigame() {
    //nothing ig
}