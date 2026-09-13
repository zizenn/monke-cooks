#include "texture.hpp"

namespace tex {
// private
void TextureManager::CreateArrayFromSize(int size) { arrayLen = size; }

// public
void TextureManager::AddTexToPaths(std::string path) {
      texPaths_.push_back(path);
}

[[nodiscard]] int TextureManager::LoadTexturesCPU() {
      int texturesLoaded = 0;
      return texturesLoaded;
}
}  // namespace tex
