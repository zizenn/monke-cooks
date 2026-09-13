#pragma once

#include <string>
#include <utility>
#include <vector>

#include "external/raylib.h"

namespace tex {

class TextureManager {
private:
      // variables
      std::vector<std::string> texPaths_;
      std::vector<std::pair<Image, std::string>> texArray_;
      int arrayLen;
      bool isImageLoaded = false;

      // functions
      void CreateArrayFromSize(int size);

public:
      void AddTexToPaths(std::string path);
      [[nodiscard]] int LoadTexturesCPU();
      [[nodiscard]] int LoadTexturesGPU();
};

}  // namespace tex
