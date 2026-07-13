#pragma once

#include <array>
#include <iostream>

namespace scene {

// main menu function prototypes
void LoadMainMenu();
void UpdateMainMenu();
void DrawMainMenu();
void UnloadMainMenu();

// game function prototypes
void LoadGame();
void UpdateGame();
void DrawGame();
void UnloadGame();

// func pointer aliases
using Func = void (*)();

enum class Scenes {
      MainMenu,
      Game,
};

class Scene {
private:
      Scenes name_;
      Func load_;
      Func update_;
      Func draw_;
      Func unload_;

public:
      Scene(const Scenes &name, const Func load, const Func update,
            const Func draw, const Func unload)
          : name_(name), load_(load), update_(update), draw_(draw),
            unload_(unload) {}

      auto GetName() const { return name_; }
      auto GetLoad() const { return load_; }
      auto GetUpdate() const { return update_; }
      auto GetDraw() const { return draw_; }
      auto GetUnload() const { return unload_; }
};

class SceneManager {
private:
      std::array<Scene, 2> scenes = {{
            {Scenes::MainMenu, LoadMainMenu, UpdateMainMenu, DrawMainMenu,
             UnloadMainMenu},
            {Scenes::Game, LoadGame, UpdateGame, DrawGame, UnloadGame},
      }};

      Scene *currentScene_ = nullptr;
      Scene *prevScene_ = nullptr;

      void Load() {
            if (currentScene_ && currentScene_->GetLoad()) {
                  currentScene_->GetLoad()();
            }
      }

      void Unload() {
            if (currentScene_ && currentScene_->GetUnload()) {
                  currentScene_->GetUnload()();
            }
      }

public:
      void Update() {
            if (currentScene_ && currentScene_->GetUpdate()) {
                  currentScene_->GetUpdate()();
            }
      }

      void Draw() {
            if (currentScene_ && currentScene_->GetDraw()) {
                  currentScene_->GetDraw()();
            }
      }
      void CheckSceneChange() {
            if (prevScene_ != currentScene_) {
                  if (prevScene_ && prevScene_->GetUnload()) {
                        prevScene_->GetUnload()();
                        std::cout << "[scene] unloading scene: "
                                  << static_cast<int>(prevScene_->GetName())
                                  << std::endl;
                  }
                  if (currentScene_ && currentScene_->GetLoad()) {
                        currentScene_->GetLoad()();
                        std::cout << "[scene] loading scene: "
                                  << static_cast<int>(currentScene_->GetName())
                                  << std::endl;
                  }
                  prevScene_ = currentScene_;
            }
      }

      void ChangeScene(const Scenes &sceneName) {
            for (auto &scene : scenes) {
                  if (scene.GetName() == sceneName) {
                        currentScene_ = &scene;
                        break;
                  }
            }
      }
};

}; // namespace scene
