#include "nlohmann/json.hpp"
#include "texture.hpp"
#include <fstream>
#include <string>

using json = nlohmann::json;

namespace tex {

void TextureManager::Load() {
      // clear old tex
      Unload();

      // open file + checks
      std::ifstream file(TextureManifestPath_);
      if (!file.is_open()) {
          TraceLog(LOG_WARNING, "[texture] could not open JSON file: %s", TextureManifestPath_.c_str());
          return;
      }

      // Check empty file
      if (file.peek() == std::ifstream::traits_type::eof()) {
          TraceLog(LOG_WARNING, "[texture] JSON file is empty: %s", TextureManifestPath_.c_str());
          return;
      }

      // transfer json into code
      json texturesJson;

      try {
          file >> texturesJson;
      } catch (const json::parse_error& e) {
          TraceLog(LOG_WARNING, "[texture] JSON parse error in %s: %s",
                   TextureManifestPath_.c_str(), e.what());
          return;
      }

      // array check
      if (!texturesJson.contains("textures") || !texturesJson["textures"].is_array()) {
            TraceLog(LOG_WARNING, "[texture] JSON missing valid \"texture array\": %s", TextureManifestPath_.c_str());
            return;
      }

      // texture loading
      for (const auto& item : texturesJson["textures"]) {

      }
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
