# pragma once

#include "external/Vector2.hpp"

namespace ui {

class Button {
private:
      // visuals
      raylib::Vector2 pos;
      int width;
      int height;
      int textureId;

      // logic
      bool isClicked = false;
      bool isHovered = false;
public:
      void Draw();
      bool DetectUpdate();
};

};
