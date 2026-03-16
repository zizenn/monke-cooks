#include "include/screen_menu.h"
#include "raylib.h"
#include "include/screen_manager.h"

bool hovered(Vector2 mousePos, Rectangle rec);
bool clicked(int mouseBtn, Vector2 mousePos, Rectangle rec);
void button(char hover_color[], char color[], char name[], char text[], Vector2 mousePos);

int main() {
  GameScreen current = MAIN_MENU;

  if (current == MAIN_MENU) {
    MenuDraw();
  }

  // close
  CloseWindow();

  return 0;

}