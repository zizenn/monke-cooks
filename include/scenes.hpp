#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// func pointer aliases
using Func = void (*)();

class Scene {
private:
      std::string name;
      Func load;
      Func update;
      Func draw;
      Func unload;

public:
      // constructor
      Scene(const std::string &name, Func load, Func update, Func draw,
            Func unload)
          : name(name), load(load), update(update), draw(draw), unload(unload) {
      }

      // name getter
      auto getName() const { return name; }

      // func getters
      auto getLoad() const { return load; }
      auto getUpdate() const { return update; }
      auto getDraw() const { return draw; }
      auto getUnload() const { return unload; }
};

class SceneManager {
private:
      std::vector<std::unique_ptr<Scene>> scenes;
      Scene *currentScene = nullptr;

public:
      void add(const Scene &scene) {
            scenes.insert(scenes.begin(), std::make_unique<Scene>(scene));
            if (currentScene == nullptr) {
                  change(scene.getName());
            }
      }

      void change(const std::string &name) {
            // unload b4 loading new scene
            if (currentScene && currentScene->getUnload()) {
                  currentScene->getUnload()();
            }

            for (auto &scenePtr : scenes) {
                  if (scenePtr->getName() == name) {
                        currentScene = scenePtr.get();
                        if (currentScene->getLoad()) {
                              currentScene->getLoad()();
                              std::cout << "scene manager, info: scene '"
                                        << name << "' loaded.\n";
                        }
                        return;
                  }
            }
            std::cerr << "scene manager, err: scene '" << name
                      << "' not found.\n";
      }

      void load() {
            if (currentScene && currentScene->getLoad()) {
                  currentScene->getLoad()();
            }
      }

      void unload() {
            if (currentScene && currentScene->getUnload()) {
                  currentScene->getUnload()();
            }
      }

      void update() {
            if (currentScene && currentScene->getUpdate()) {
                  currentScene->getUpdate()();
            }
      }

      void draw() {
            if (currentScene && currentScene->getDraw()) {
                  currentScene->getDraw()();
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
