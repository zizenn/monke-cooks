#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// func pointer aliases
using Func = void (*)();

class Scene {
private:
      std::string name_;
      Func load_;
      Func update_;
      Func draw_;
      Func unload_;

public:
      // constructor
      Scene(const std::string &name, Func load, Func update, Func draw,
            Func unload)
          : name_(name), load_(load), update_(update), draw_(draw),
            unload_(unload) {}

      // name getter
      auto getName() const { return name_; }

      // func getters
      auto getLoad() const { return load_; }
      auto getUpdate() const { return update_; }
      auto getDraw() const { return draw_; }
      auto getUnload() const { return unload_; }
};

class SceneManager {
private:
      std::vector<std::unique_ptr<Scene>> scenes;
      Scene *currentScene = nullptr;

      void Load() {
            if (currentScene && currentScene->getLoad()) {
                  currentScene->getLoad()();
            }
      }

      void Unload() {
            if (currentScene && currentScene->getUnload()) {
                  currentScene->getUnload()();
            }
      }

      void Update() {
            if (currentScene && currentScene->getUpdate()) {
                  currentScene->getUpdate()();
            }
      }

      void Draw() {
            if (currentScene && currentScene->getDraw()) {
                  currentScene->getDraw()();
            }
      }

public:
      void PushScene(std::unique_ptr<Scene> scene) {
            if (condition) {
            }
      }
};
// scene function prototypes

void LoadMainMenu();
void UpdateMainMenu();
void DrawMainMenu();
void UnloadMainMenu();

// game function prototypes
void LoadGame();
void UpdateGame();
void DrawGame();
void UnloadGame();
