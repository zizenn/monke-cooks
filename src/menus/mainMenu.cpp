#include <array>
#include "Color.hpp"
#include "Rectangle.hpp"
#include "raylib.h"
#include "scenes.hpp"
#include "time.hpp"

static float posX = 0.0f;

std::array<raylib::Rectangle, 1> rects = {
      (raylib::Rectangle){0, 2, 12, 20},
};

void scene::LoadMainMenu() {
}

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

void scene::UnloadMainMenu() { return; }
