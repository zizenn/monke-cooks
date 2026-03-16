#include "include/screen_menu.h"
#include "raylib.h"
#include "include/screen_manager.h"
#include "include/globals.h"

static Music menu_music;
static Sound clickSound;
static Texture2D monke;

static Rectangle loadBtn        = { 15, 650,  200, 50 };
static Rectangle advancementBtn = { 230, 650, 200, 50 };
static Rectangle restartBtn     = { 445, 650, 200, 50 };
static Rectangle quitBtn        = { 660, 650, 200, 50 };

void InitCampaignMenu(void);
void UpdateCampaignMenu(void);
void DrawCampaignMenu(void);
void UnloadCampaignMenu(void);

void button(char hover_color[], char color[], char name[], char text[]);


//Sakif pls add implementation to detect more buttons more efficiently
void UpdateCampaignMenu() {
  UpdateMusicStream(menu_music);
   if (clicked(MOUSE_BUTTON_LEFT, quitBtn)) {
    PlaySound(clickSound);
    current = MAIN_MENU;
   }
   
   if (clicked(MOUSE_BUTTON_LEFT, restartBtn)) {
    PlaySound(clickSound);
   }

    if (clicked(MOUSE_BUTTON_LEFT, advancementBtn)) {
    PlaySound(clickSound);
    // current = ADVANCEMENTS_MENU;
   }

   if (clicked(MOUSE_BUTTON_LEFT, loadBtn)) {
    PlaySound(clickSound);
   }
}

void DrawCampaignMenu(void) {
  ClearBackground(RAYWHITE);
  DrawText("Campaign Mode", 15, 15, 35, BLACK);

  // loadBtn
  Color startBtnColor = hovered(loadBtn) ? DARKGRAY : GRAY;
  DrawRectangleRec(loadBtn, startBtnColor);
  DrawText("Start/Continue", loadBtn.x + 20,loadBtn.y + 15, 20, WHITE); //implement first-time detection when save states are added

  // advancementBtn
  Color multiplayerBtnColor = hovered(advancementBtn) ? DARKGRAY : GRAY;
  DrawRectangleRec(advancementBtn, multiplayerBtnColor);
  DrawText("Advancements", advancementBtn.x + 25,advancementBtn.y + 15, 20, WHITE);

  // restartBtn
  Color settingsBtnColor = hovered(restartBtn) ? DARKGRAY : GRAY;
  DrawRectangleRec(restartBtn, settingsBtnColor);
  DrawText("[DANGER] Reset", restartBtn.x + 20, restartBtn.y + 15, 20, WHITE); 

  // quitBtn
  Color quitBtnColor = hovered(quitBtn) ? DARKGRAY : GRAY;
  DrawRectangleRec(quitBtn, quitBtnColor);
  DrawText("Back", quitBtn.x + 45, quitBtn.y + 15, 20, WHITE);
}

void InitCampaignMenu() {
  menu_music = LoadMusicStream("assets/brackeys/music/time_for_adventure.mp3");
  clickSound = LoadSound("assets/brackeys/sounds/tap.wav");
  monke = LoadTexture("assets/monke_front.png");
  PlayMusicStream(menu_music);
}

void UnloadCampaignMenu() {
    UnloadMusicStream(menu_music);
    UnloadTexture(monke);
    UnloadSound(clickSound);
}