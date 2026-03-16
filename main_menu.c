#include "include/screen_menu.h"
#include "raylib.h"
#include "include/screen_manager.h"
#include "include/globals.h"

static Music menu_music;
static Sound clickSound;
static Texture2D monke;

static Rectangle campaignBtn    = { 15, 70,  200, 50 };
static Rectangle multiplayerBtn = { 15, 150, 200, 50 };
static Rectangle settingsBtn    = { 15, 230, 200, 50 };
static Rectangle quitBtn        = { 15, 310, 200, 50 };

void InitMainMenu(void);
void UpdateMainMenu(void);
void DrawMainMenu(void);
void UnloadMainMenu(void);

void button(char hover_color[], char color[], char name[], char text[]);


//Sakif pls add implementation to detect more buttons more efficiently
void UpdateMainMenu() {
  UpdateMusicStream(menu_music);
   if (clicked(MOUSE_BUTTON_LEFT, quitBtn)) {
     PlaySound(clickSound);
     shouldQuit = true;
   }

  if (clicked(MOUSE_LEFT_BUTTON, campaignBtn)) {
     PlaySound(clickSound);
     current = CAMPAIGN_MENU;
   }

  if (clicked(MOUSE_BUTTON_LEFT, settingsBtn)) {
     PlaySound(clickSound);
     // current = SETTINGS_MENU;
   }

  if (clicked(MOUSE_BUTTON_LEFT, multiplayerBtn)) {
     PlaySound(clickSound);
     // current = MULTIPLAYER_MENU;
   }
}

void DrawMainMenu(void) {
  ClearBackground(RAYWHITE);
  DrawText("monke cooks", 15, 15, 35, BLACK);

  // campaignBtn
  Color campaignBtnColor = hovered(campaignBtn) ? DARKGRAY : GRAY;
  DrawRectangleRec(campaignBtn, campaignBtnColor);
  DrawText("campaign", campaignBtn.x + 45,campaignBtn.y + 15, 20, WHITE);

  // multiplayerBtn
  Color multiplayerBtnColor = hovered(multiplayerBtn) ? DARKGRAY : GRAY;
  DrawRectangleRec(multiplayerBtn, multiplayerBtnColor);
  DrawText("multiplayer", multiplayerBtn.x + 45,multiplayerBtn.y + 15, 20, WHITE);

  // settingsBtn
  Color settingsBtnColor = hovered(settingsBtn) ? DARKGRAY : GRAY;
  DrawRectangleRec(settingsBtn, settingsBtnColor);
  DrawText("settings", settingsBtn.x + 45, settingsBtn.y + 15, 20, WHITE); 

  // quitBtn
  Color quitBtnColor = hovered(quitBtn) ? DARKGRAY : GRAY;
  DrawRectangleRec(quitBtn, quitBtnColor);
  DrawText("quit", quitBtn.x + 45, quitBtn.y + 15, 20, WHITE);
}

void InitMainMenu() {
  menu_music = LoadMusicStream("assets/brackeys/music/time_for_adventure.mp3");
  clickSound = LoadSound("assets/brackeys/sounds/tap.wav");
  monke = LoadTexture("assets/monke_front.png");
  PlayMusicStream(menu_music);
}

void UnloadMainMenu() {
    UnloadMusicStream(menu_music);
    UnloadTexture(monke);
    UnloadSound(clickSound);
}