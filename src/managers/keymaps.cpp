#include "keymaps.hpp"

#include <fstream>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"
#include "raylib.h"

using json = nlohmann::json;

namespace keymaps {

namespace {

const std::string KEYMAP_FILEPATH = "playerData/keymap.json";
std::vector<std::pair<int, Action>> activeControls;
bool loaded = false;

struct KeyEntry {
      const char* name;
      int keycode;
};

struct ActionEntry {
      const char* name;
      Action action;
};

constexpr KeyEntry KEY_TABLE[] = {
      {"A", KEY_A},
      {"B", KEY_B},
      {"C", KEY_C},
      {"D", KEY_D},
      {"E", KEY_E},
      {"F", KEY_F},
      {"G", KEY_G},
      {"H", KEY_H},
      {"I", KEY_I},
      {"J", KEY_J},
      {"K", KEY_K},
      {"L", KEY_L},
      {"M", KEY_M},
      {"N", KEY_N},
      {"O", KEY_O},
      {"P", KEY_P},
      {"Q", KEY_Q},
      {"R", KEY_R},
      {"S", KEY_S},
      {"T", KEY_T},
      {"U", KEY_U},
      {"V", KEY_V},
      {"W", KEY_W},
      {"X", KEY_X},
      {"Y", KEY_Y},
      {"Z", KEY_Z},

      {"0", KEY_ZERO},
      {"1", KEY_ONE},
      {"2", KEY_TWO},
      {"3", KEY_THREE},
      {"4", KEY_FOUR},
      {"5", KEY_FIVE},
      {"6", KEY_SIX},
      {"7", KEY_SEVEN},
      {"8", KEY_EIGHT},
      {"9", KEY_NINE},

      {"F1", KEY_F1},
      {"F2", KEY_F2},
      {"F3", KEY_F3},
      {"F4", KEY_F4},
      {"F5", KEY_F5},
      {"F6", KEY_F6},
      {"F7", KEY_F7},
      {"F8", KEY_F8},
      {"F9", KEY_F9},
      {"F10", KEY_F10},
      {"F11", KEY_F11},
      {"F12", KEY_F12},

      {"SPACE", KEY_SPACE},
      {"ESCAPE", KEY_ESCAPE},
      {"ENTER", KEY_ENTER},
      {"TAB", KEY_TAB},
      {"BACKSPACE", KEY_BACKSPACE},
      {"INSERT", KEY_INSERT},
      {"DELETE", KEY_DELETE},
      {"HOME", KEY_HOME},
      {"END", KEY_END},
      {"PAGE_UP", KEY_PAGE_UP},
      {"PAGE_DOWN", KEY_PAGE_DOWN},
      {"CAPS_LOCK", KEY_CAPS_LOCK},
      {"SCROLL_LOCK", KEY_SCROLL_LOCK},
      {"NUM_LOCK", KEY_NUM_LOCK},
      {"PRINT_SCREEN", KEY_PRINT_SCREEN},
      {"PAUSE", KEY_PAUSE},

      {"UP", KEY_UP},
      {"DOWN", KEY_DOWN},
      {"LEFT", KEY_LEFT},
      {"RIGHT", KEY_RIGHT},

      {"LEFT_SHIFT", KEY_LEFT_SHIFT},
      {"LEFT_CONTROL", KEY_LEFT_CONTROL},
      {"LEFT_ALT", KEY_LEFT_ALT},
      {"LEFT_SUPER", KEY_LEFT_SUPER},
      {"RIGHT_SHIFT", KEY_RIGHT_SHIFT},
      {"RIGHT_CONTROL", KEY_RIGHT_CONTROL},
      {"RIGHT_ALT", KEY_RIGHT_ALT},
      {"RIGHT_SUPER", KEY_RIGHT_SUPER},

      {"KP_0", KEY_KP_0},
      {"KP_1", KEY_KP_1},
      {"KP_2", KEY_KP_2},
      {"KP_3", KEY_KP_3},
      {"KP_4", KEY_KP_4},
      {"KP_5", KEY_KP_5},
      {"KP_6", KEY_KP_6},
      {"KP_7", KEY_KP_7},
      {"KP_8", KEY_KP_8},
      {"KP_9", KEY_KP_9},

      {"APOSTROPHE", KEY_APOSTROPHE},
      {"COMMA", KEY_COMMA},
      {"MINUS", KEY_MINUS},
      {"PERIOD", KEY_PERIOD},
      {"SLASH", KEY_SLASH},
      {"SEMICOLON", KEY_SEMICOLON},
      {"EQUAL", KEY_EQUAL},
      {"LEFT_BRACKET", KEY_LEFT_BRACKET},
      {"BACKSLASH", KEY_BACKSLASH},
      {"RIGHT_BRACKET", KEY_RIGHT_BRACKET},
      {"GRAVE", KEY_GRAVE},
      {"KB_MENU", KEY_KB_MENU},
};

constexpr ActionEntry ACTION_TABLE[] = {
      {"move_up", Action::MOVE_UP},     {"move_down", Action::MOVE_DOWN},
      {"move_left", Action::MOVE_LEFT}, {"move_right", Action::MOVE_RIGHT},
      {"interact", Action::INTERACT},   {"pause", Action::PAUSE},
};

int KeyNameToCode(const std::string& name) {
      for (const auto& entry : KEY_TABLE) {
            if (name == entry.name) return entry.keycode;
      }
      return KEY_NULL;
}

Action ActionNameToEnum(const std::string& name) {
      for (const auto& entry : ACTION_TABLE) {
            if (name == entry.name) return entry.action;
      }
      return Action::NULL_ACTION;
}

void WriteDefaultConfig() {
      json defaultConfig = {
            {"W", "move_up"},    {"S", "move_down"},    {"A", "move_left"},
            {"D", "move_right"}, {"SPACE", "interact"}, {"ESCAPE", "pause"},
      };

      std::ofstream file(KEYMAP_FILEPATH);
      if (!file.is_open()) {
            TraceLog(LOG_WARNING,
                     "[keymaps] Could not create default config at %s",
                     KEYMAP_FILEPATH.c_str());
            return;
      }
      file << defaultConfig.dump(6);
}

}  // anonymous namespace

Action Keymaps::currentAction_ = Action::NULL_ACTION;

void Keymaps::LoadFromJson() {
      activeControls.clear();

      std::ifstream file(KEYMAP_FILEPATH);
      if (!file.is_open() || file.peek() == std::ifstream::traits_type::eof()) {
            TraceLog(LOG_INFO,
                     "[keymaps] no config found, writing defaults to %s",
                     KEYMAP_FILEPATH.c_str());
            if (file.is_open()) file.close();
            WriteDefaultConfig();
            file.open(KEYMAP_FILEPATH);
            if (!file.is_open()) return;
      }

      json config;
      try {
            file >> config;
      } catch (const json::parse_error& e) {
            TraceLog(LOG_WARNING, "[keymaps] JSON parse error in %s: %s",
                     KEYMAP_FILEPATH.c_str(), e.what());
            return;
      }

      for (auto it = config.begin(); it != config.end(); ++it) {
            const std::string& keyName = it.key();
            const std::string& actionName = it.value();

            const int keycode = KeyNameToCode(keyName);
            if (keycode == KEY_NULL) {
                  TraceLog(LOG_WARNING,
                           "[keymaps] Unknown key \"%s\" in config — skipping",
                           keyName.c_str());
                  continue;
            }

            const Action action = ActionNameToEnum(actionName);
            if (action == Action::NULL_ACTION) {
                  TraceLog(
                        LOG_WARNING,
                        "[keymaps] Unknown action \"%s\" in config — skipping",
                        actionName.c_str());
                  continue;
            }

            activeControls.emplace_back(keycode, action);
      }
}

Action Keymaps::CheckFrameAction_() {
      for (const auto& [keycode, action] : activeControls) {
            if (IsKeyPressed(keycode)) {
                  currentAction_ = action;
                  return action;
            }
      }
      currentAction_ = Action::NULL_ACTION;
      return Action::NULL_ACTION;
}

const Action& Keymaps::GetCurrentAction() { return currentAction_; }

Action Keymaps::Update() {
      if (!loaded) {
            LoadFromJson();
            loaded = true;
      }
      return CheckFrameAction_();
}

}  // namespace keymaps
