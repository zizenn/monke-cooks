#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "external/raylib.h"

namespace tex {

struct TexturePacket {
      std::string name;
      Texture2D tex;
};

class TextureManager {
private:
      std::string TextureManifestPath_;
      std::vector<TexturePacket> loadedTexArray_;
      bool isSet = false;
      bool isLoaded = false;

public:
      // constructors
      TextureManager(std::string filePath) : TextureManifestPath_(filePath) {
            isSet = true;
            Load();
      }
      TextureManager() = default;

      // deconstructors
      ~TextureManager() { Unload(); }

      // functions
      void Set(std::string filePath) {
            TextureManifestPath_ = std::move(filePath);
      };

      void Load();
      [[nodiscard]] Texture2D* GetTexture(std::string& name);
      void Unload();
};

}  // namespace tex
