#include "nlohmann/json.hpp"
#include "texture.hpp"
#include <fstream>
#include <queue>
#include <string>

using json = nlohmann::json;

namespace tex {

std::queue<std::string> TextureManager::DecodeFilePathsFromJSON_() {
      std::queue<std::string> texturePaths;

      std::ifstream file(TextureManifestPath_);
      if (!file.is_open() || file.peek() == std::ifstream::traits_type::eof()) {
            if (file.is_open()) file.close();
            return  texturePaths;
      }

      json textures;
      try {
            file >> textures;
      } catch (const json::parse_error& e) {
            TraceLog(LOG_WARNING, "[texture] JSON parse error in %s: %s",
                     TextureManifestPath_.c_str(), e.what());
            return texturePaths;
      }

      return texturePaths;
}


//      // private
//      void TextureManager::CreateArrayFromSize(int size) {
//            arrayLen_ = size;
//      }
//
//      // public
//      void TextureManager::AddTexToPaths(std::string path) {
//            texPaths_.push_back(path);
//      }
//
//      [[nodiscard]] int TextureManager::LoadTextures() {
//            int texturesLoaded = 0;
//            return texturesLoaded;
//      }
}
