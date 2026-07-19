#pragma once

namespace keymaps {

enum class Action {
      NULL_ACTION,
      MOVE_UP,
      MOVE_DOWN,
      MOVE_LEFT,
      MOVE_RIGHT,
      INTERACT,
      PAUSE
};

class Keymaps {
public:
      Keymaps() = delete;

      static Action Update();
      static const Action& GetCurrentAction();

private:
      static void LoadFromJson();
      static Action CheckFrameAction_();
      static Action currentAction_;
};

} // namespace keymaps
