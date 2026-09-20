#pragma once

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

public:
      TextureManager(std::string filePath) : TextureManifestPath_(filePath) {
            Load();
      }
      ~TextureManager() { Unload(); }

      // functions
      void Load();

      [[nodiscard]] Texture2D* GetTexture(std::string& name);

      void Unload();
};

}  // namespace tex
