#include "ui.hpp"
#include "Vector2.hpp"

namespace ui {
      void Button::Draw() {
            // use rectangle draw function with the texture overload
      }

      bool Button::DetectUpdate() {
            raylib::Vector2 mousePos = GetMousePosition();
            bool tempClick = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
            if (CheckCollisionPointRec(mousePos, buttonRect)) {
                  isHovered = true;
            } else if (CheckCollisionPointRec(mousePos, buttonRect) && tempClick == true) {
                  isClicked = true;
                  isHovered = true;
            } else {
                  isHovered = false;
                  isClicked = false;
            }
            return false;
      }
};
