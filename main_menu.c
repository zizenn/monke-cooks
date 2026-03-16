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

void InitMenu(void);
void UpdateMenu(void);
void DrawMenu(void);
void UnloadMenu(void);

bool hovered(Rectangle rec);
bool clicked(int mouseBtn, Rectangle rec);
void button(char hover_color[], char color[], char name[], char text[]);

void UpdateMenu() {
  UpdateMusicStream(menu_music);
   if (clicked(MOUSE_BUTTON_LEFT, quitBtn)) {
     PlaySound(clickSound);
     CloseWindow();
   }

   if (clicked(MOUSE_BUTTON_LEFT, campaignBtn)) {
     current = CAMPAIGN_MENU;
   }
}

void DrawMenu(void) {
   
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

void InitMenu() {
  menu_music = LoadMusicStream("assets/brackeys/music/time_for_adventure.mp3");
  clickSound = LoadSound("assets/brackeys/sounds/tap.wav");
  monke = LoadTexture("assets/monke_front.png");
  PlayMusicStream(menu_music);
}

void UnloadMenu() {
    UnloadMusicStream(menu_music);
    UnloadTexture(monke);
    UnloadSound(clickSound);
}

bool hovered(Rectangle rec) {
  return CheckCollisionPointRec(mousePos, rec); 
}

bool clicked(int mouseBtn, Rectangle rec) {
  return IsMouseButtonPressed(mouseBtn) && hovered(rec);
}