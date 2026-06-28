#pragma once

#include <iostream>
#include <string>
#include <vector>

// func pointer aliases
using VoidFunc = void (*)();
using UpdateFunc = void (*)(float);

class Scene {
private:
  std::string name;
  VoidFunc load;
  UpdateFunc update;
  VoidFunc draw;
  VoidFunc unload;

public:
  // constructor
  Scene(const std::string &name, VoidFunc load, UpdateFunc update,
        VoidFunc draw, VoidFunc unload)
      : name(name), load(load), update(update), draw(draw), unload(unload) {}

  // name getter
  auto getName() const { return name; }

  // func getters
  auto getLoad() const { return load; }
  auto getUpdate() const { return update; }
  auto getDraw() const { return draw; }
  auto getUnload() const { return unload; } // <-- Added
};

class SceneManager {
private:
  std::vector<Scene> scenes;
  Scene *currentScene = nullptr;

public:
  void addScene(const Scene &scene) {
    scenes.push_back(scene);
    if (currentScene == nullptr) {
      changeScene(scene.getName());
    }
  }

  void changeScene(const std::string &name) {
    // unload b4 loading new scene
    if (currentScene && currentScene->getUnload()) {
      currentScene->getUnload()();
    }

    for (auto &scene : scenes) {
      if (scene.getName() == name) {
        currentScene = &scene;
        if (currentScene->getLoad()) {
          currentScene->getLoad()();
          std::cout << "scene manager, info: scene '" << name << "' loaded.\n";
        }
        return;
      }
    }
    std::cerr << "scene manager, err: scene '" << name << "' not found.\n";
  }
};
