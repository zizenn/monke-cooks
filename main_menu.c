#include "include/screen_menu.h"
#include "raylib.h"
#include "include/screen_manager.h"

// music
static Music menu_music;
// sounds
static Sound clickSound;
// textures
static Texture2D monke;

void InitMenu(void);
GameScreen UpdateMenu(void);
void DrawMenu(void);
void UnloadMenu(void);

bool hovered(Vector2 mousePos, Rectangle rec);
bool clicked(int mouseBtn, Vector2 mousePos, Rectangle rec);
void button(char hover_color[], char color[], char name[], char text[], Vector2 mousePos);

void DrawMenu(void) {

  PlayMusicStream(menu_music);

  // -- VARIABLES --
  
  // music
  menu_music = LoadMusicStream("assets/brackeys/music/time_for_adventure.mp3");
  
  // sounds
  clickSound = LoadSound("assets/brackeys/sounds/tap.wav");
  
  // textures
  monke = LoadTexture("assets/monke_front.png");

  // buttons
  Rectangle campaignBtn = { 15, 70, 200, 50 };
  Rectangle multiplayerBtn = { 15, 150, 200, 50 };
  Rectangle settingsBtn = { 15, 230, 200, 50 };
  Rectangle quitBtn = { 15, 310, 200, 50 };

  // run

  while (!WindowShouldClose()) {

    // UpdateMusicStream(menu_music);

    Vector2 mousePos = GetMousePosition();

    if (clicked(MOUSE_BUTTON_LEFT, mousePos, quitBtn)) {
      // PlaySound(clickSound);
      break;
    }

    if (clicked(MOUSE_BUTTON_LEFT, mousePos, campaignBtn)) {
      continue;
    }

   BeginDrawing();

    ClearBackground(RAYWHITE);
    DrawText("monke cooks", 15, 15, 35, BLACK);

    // campaignBtn
    Color campaignBtnColor = hovered(mousePos, campaignBtn) ? DARKGRAY : GRAY;
    DrawRectangleRec(campaignBtn, campaignBtnColor);
    DrawText("campaign", campaignBtn.x + 45,campaignBtn.y + 15, 20, WHITE);

    // campaignBtn
    Color multiplayerBtnColor = hovered(mousePos, multiplayerBtn) ? DARKGRAY : GRAY;
    DrawRectangleRec(multiplayerBtn, multiplayerBtnColor);
    DrawText("multiplayer", multiplayerBtn.x + 45,multiplayerBtn.y + 15, 20, WHITE);

    // settingsBtn
    Color settingsBtnColor = hovered(mousePos, settingsBtn) ? DARKGRAY : GRAY;
    DrawRectangleRec(settingsBtn, settingsBtnColor);
    DrawText("settings", settingsBtn.x + 45, settingsBtn.y + 15, 20, WHITE); 

    // quitBtn
    Color quitBtnColor = hovered(mousePos, quitBtn) ? DARKGRAY : GRAY;
    DrawRectangleRec(quitBtn, quitBtnColor);
    DrawText("quit", quitBtn.x + 45, quitBtn.y + 15, 20, WHITE);

  EndDrawing();
  }
 
}

void UnloadMenu() {
    UnloadMusicStream(menu_music);
    UnloadTexture(monke);
    UnloadSound(clickSound);
}

GameScreen MenuUpdate(void) {
    
}

bool hovered(Vector2 mousePos, Rectangle rec) {
  return CheckCollisionPointRec(mousePos, rec); 
}

bool clicked(int mouseBtn, Vector2 mousePos, Rectangle rec) {
  return IsMouseButtonPressed(mouseBtn) && hovered(mousePos, rec);
}