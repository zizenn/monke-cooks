#include "external/raylib.h"
#include "external/raygui.h"
#include "external/raymath.h"
#include "game/globals.h"
#include "game/game.h"
#include "game/config.h"
#include "game/display_screen.h"
#include "minigames/basketcatch.h"
#include "stdbool.h"

BasketCatchResult basketCatchResult = BASKET_CATCH_RUNNING;

static const float FIELD_X = BASKET_FIELD_X;
static const float FIELD_Y = BASKET_FIELD_Y;

static const int BASKET_WIDTH_C = BASKET_WIDTH;
static const int BASKET_HEIGHT_C = BASKET_HEIGHT;

static const float OBJECT_SPACING = BASKET_OBJECT_SPACING;
static const int OBJECT_WIDTH = BASKET_OBJECT_WIDTH;
static const int OBJECT_HEIGHT = BASKET_OBJECT_HEIGHT;

static const int MAX_MISSES = BASKET_MAX_MISSES;

typedef struct {
    int x;
    float y;
    float velocity;
    bool active;
    bool hit;
} Object;

static int fieldWidth = 200;
static int fieldHeight = 300;

static int CenterX = 0;
static int CenterY = 0;

static int basketX = 0; // -1 Left, 0 Center, 1 Right
static const int BASKET_OFFSET_C = BASKET_OFFSET;
static float basketY;

static Vector2 displayBasketPosition = {0, 0};

static Object objects[BASKET_OBJECT_COUNT];
static float objectSpeed = BASKET_INITIAL_OBJECT_SPEED;
static int objectsMissed = 0;
static int objectsCaught = 0;
static float missCooldown = 0;


static void SpawnObjects() {
    float objectY = FIELD_Y;
    for (int i = 0; i < BASKET_OBJECT_COUNT; i++) {
        objectY -= OBJECT_SPACING;
        objects[i].x = GetRandomValue(-1, 1);
        objects[i].y = objectY;
        objects[i].active = true;
        objects[i].hit = false;
    }
}

void InitBasketMinigame() {
    fieldWidth = 200;
    fieldHeight = panelBounds.height;
    basketX = 0;
    basketY = FIELD_Y + fieldHeight - BASKET_OFFSET;
    basketCatchResult = BASKET_CATCH_RUNNING;
    objectSpeed = 100.0f;
    objectsMissed = 0;
    objectsCaught = 0;
    missCooldown = 0;
    CenterX = panelBounds.x + panelBounds.width/2;
    CenterY = panelBounds.y + panelBounds.height/2;
    SpawnObjects();

}

void UpdateBasketMinigame() {
    float deltaTime = GetFrameTime();
    missCooldown -= deltaTime;
    objectSpeed += BASKET_OBJECT_SPEED_INCREMENT * deltaTime;

    bool anyActive = false;
    for (int i = 0; i < BASKET_OBJECT_COUNT; i++) {
        if(!objects[i].active) continue;
        anyActive = true;
        objects[i].y += objectSpeed * deltaTime;

        if (objects[i].y >= FIELD_Y + fieldHeight + OBJECT_HEIGHT) {
            objects[i].active = false;
            if (missCooldown <= 0.0f) missCooldown = BASKET_MISS_COOLDOWN;
            objectsMissed++;
        }
    }

    if (missCooldown <= 0.0f) {
        if (IsKeyDown(KEY_A)) {
            basketX = -1;
        } else if (IsKeyDown(KEY_D)) {
            basketX = 1;
        } else {
            basketX = 0;
        }
    }
    displayBasketPosition.x = Lerp(displayBasketPosition.x, basketX, 10.0f * deltaTime);

    if (missCooldown <= 0.0f) {
        for (int i = 0; i < BASKET_OBJECT_COUNT; i++) {
            if (!objects[i].active) continue;
            if (objects[i].y >= basketY + BASKET_HEIGHT / 2 && objects[i].x == basketX) {
                objects[i].active = false;
                objects[i].hit = true;
                break;
            }
        }
    }

    bool allInactive = true;
    objectsCaught = 0;
    for (int i = 0; i < BASKET_OBJECT_COUNT; i++) {
        if (objects[i].hit) {
            objectsCaught++;
        }
        if (objects[i].active) {
            allInactive = false;
        }
    }

    if (objectsCaught >= BASKET_OBJECT_COUNT - MAX_MISSES && allInactive) {
        basketCatchResult = BASKET_CATCH_WIN;
        SummonNotif("YOU WIN!", SUCCESS);
    } else if (objectsCaught != BASKET_OBJECT_COUNT && allInactive) {
        basketCatchResult = BASKET_CATCH_LOSE;
        SummonNotif("YOU LOSE!", ERROR);
    }
}

void DrawBasketMinigame() {
    //field
    DrawRectangle(panelBounds.x + FIELD_X, panelBounds.y + FIELD_Y, fieldWidth, fieldHeight, GRAY);

    //objects
    for (int i = 0; i < BASKET_OBJECT_COUNT; i++) {
        if (!objects[i].active || objects[i].y < FIELD_Y || objects[i].y > FIELD_Y + fieldHeight) continue;
        DrawRectangle(panelBounds.x + FIELD_X + (fieldWidth / 2) + ((fieldWidth - 80) / 2 * objects[i].x) - OBJECT_WIDTH / 2, objects[i].y + panelBounds.y + FIELD_Y - OBJECT_HEIGHT / 2, OBJECT_WIDTH, OBJECT_HEIGHT, GREEN);
    }

    //basket
    DrawRectangle(panelBounds.x + FIELD_X + (fieldWidth / 2) + ((fieldWidth - 80) / 2 * basketX) - BASKET_WIDTH / 2, panelBounds.y + basketY, BASKET_WIDTH, BASKET_HEIGHT, (Color){0, 0, 0, 50});
    if (missCooldown <= 0.0f) { 
        DrawRectangle(panelBounds.x + FIELD_X + (fieldWidth / 2) + ((fieldWidth - 80) / 2 * displayBasketPosition.x) - BASKET_WIDTH / 2, panelBounds.y + basketY, BASKET_WIDTH, BASKET_HEIGHT, BLACK);
    } else {
        DrawRectangle(panelBounds.x + FIELD_X + (fieldWidth / 2) + ((fieldWidth - 80) / 2 * displayBasketPosition.x) - BASKET_WIDTH / 2, panelBounds.y + basketY, BASKET_WIDTH, BASKET_HEIGHT, RED);
    }
    Color LossColor = objectsMissed <= MAX_MISSES ? WHITE : RED;
    DrawText(TextFormat("Strikes: %d/%d", objectsMissed, MAX_MISSES + 1), panelBounds.x + panelBounds.width / 2 - 70, panelBounds.y + panelBounds.height - 10, 20, LossColor);
}

void UnloadBasketMinigame() {
    //nothing
}