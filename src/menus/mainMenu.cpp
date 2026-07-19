#include "Color.hpp"
#include "Rectangle.hpp"
#include "raylib.h"
#include "scenes.hpp"
#include "time.hpp"
#include <iostream>

static float posX = 0.0f;

namespace {

Rectangle rects[1];

}

void scene::LoadMainMenu() { rects[0] = {0, 0, 100, 50}; }

void scene::UpdateMainMenu() {
      posX += 100.0f * Time::DeltaTime();
      if (posX > 1280) {
            posX = 0.0f;
      }
}

void scene::DrawMainMenu() {
      raylib::Rectangle testRec = {posX, 20, 100, 100};
      testRec.Draw(raylib::Color::SkyBlue());
}

void scene::UnloadMainMenu() {
      std::cout << "Unloading Main Menu" << std::endl;
}
