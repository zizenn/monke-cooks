# pragma once

#include <functional>
#include "external/Rectangle.hpp"

namespace ui {

struct ButtonTexturePacket {
      int baseTextureId;
      int hoveredTextureId;
      int clickedTextureId;
};

class Button {
private:
      // visuals
      raylib::Rectangle buttonRect;
      ButtonTexturePacket textures;

      // logic
      bool isClicked = false;
      bool isHovered = false;
      std::function<void()> onClick;
public:
      Button(float x, float y, float width, float height, int basetextureId, int hoveredtextureId, int clickedtextureId, std::function<void()> function) {
            buttonRect.x = x;
            buttonRect.y = y;
            buttonRect.width = width;
            buttonRect.height = height;
            textures.baseTextureId= basetextureId;
            textures.hoveredTextureId = hoveredtextureId;
            textures.clickedTextureId = clickedtextureId;
            onClick = function;
      };

      Button(raylib::Rectangle rect, ButtonTexturePacket texturePacket, std::function<void()> function) {
            buttonRect = rect;
            textures = texturePacket;
            onClick = function;
      }

      void Draw();
      bool DetectUpdate();
};

};
