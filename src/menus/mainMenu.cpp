#include "Color.hpp"
#include "Rectangle.hpp"
#include "raylib-cpp.hpp"
#include "scenes.hpp"

void LoadMainMenu() { std::cout << "Loading Main Menu" << std::endl; }

void UpdateMainMenu() {}

void DrawMainMenu() {
      raylib::Rectangle testRec = {20, 20, 100, 100};
      testRec.Draw(raylib::Color::SkyBlue());
}

void UnloadMainMenu() { std::cout << "Unloading Main Menu" << std::endl; }
