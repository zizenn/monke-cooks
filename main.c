#include <raylib.h>

bool hovered(Vector2 mousePos, Rectangle rec);
bool clicked(int mouseBtn, Vector2 mousePos, Rectangle rec);
void button(char hover_color[], char color[], char name[], char text[], Vector2 mousePos);

int main() {

  // init
  InitWindow(1280, 720, "monke cooks");
  InitAudioDevice();
  SetTargetFPS(60);

  // -- VARIABLES --

  // music
  Music menu_music = LoadMusicStream("assets/brackeys/music/time_for_adventure.mp3");
  PlayMusicStream(menu_music);

  // sounds
  Sound clickSound = LoadSound("assets/brackeys/sounds/tap.wav");

  // textures
  Texture2D monke = LoadTexture("assets/monke_front.png");

  // buttons
  Rectangle playBtn = { 15, 70, 200, 50 };
  Rectangle settingsBtn = { 15, 150, 200, 50 };
  Rectangle quitBtn = { 15, 230, 200, 50 };

  // run

  while (!WindowShouldClose()) {

    UpdateMusicStream(menu_music);

    Vector2 mousePos = GetMousePosition();

    if (clicked(MOUSE_BUTTON_LEFT, mousePos, quitBtn)) {
      PlaySound(clickSound);
      break;
    }

   BeginDrawing();

    ClearBackground(RAYWHITE);
    DrawText("monke cooks", 15, 15, 35, BLACK);

    // playBtn
    Color playBtnColor = hovered(mousePos, playBtn) ? DARKGRAY : GRAY;
    DrawRectangleRec(playBtn, playBtnColor);
    DrawText("play", playBtn.x + 45, playBtn.y + 15, 20, WHITE);

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
 

  // close
  UnloadMusicStream(menu_music);
  UnloadTexture(monke);
  CloseWindow();

  return 0;

}

bool hovered(Vector2 mousePos, Rectangle rec) {
  return CheckCollisionPointRec(mousePos, rec); 
}

bool clicked(int mouseBtn, Vector2 mousePos, Rectangle rec) {
  return IsMouseButtonPressed(mouseBtn) && hovered(mousePos, rec);
}