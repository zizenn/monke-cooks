#pragma once

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
      raylib::Rectangle buttonRect_;
      ButtonTexturePacket textures_;

      // logic
      bool isClicked = false;
      bool isHovered = false;
      std::function<void()> onClick_;

public:
      Button(float x, float y, float width, float height, int basetextureId,
             int hoveredtextureId, int clickedtextureId,
             std::function<void()> function) {
            buttonRect_.x = x;
            buttonRect_.y = y;
            buttonRect_.width = width;
            buttonRect_.height = height;
            textures_.baseTextureId = basetextureId;
            textures_.hoveredTextureId = hoveredtextureId;
            textures_.clickedTextureId = clickedtextureId;
            onClick_ = function;
      };

      Button(raylib::Rectangle rect, ButtonTexturePacket texturePacket,
             std::function<void()> function) {
            buttonRect_ = rect;
            textures_ = texturePacket;
            onClick_ = function;
      }

      void Draw();
      bool DetectUpdate();
};

};  // namespace ui
