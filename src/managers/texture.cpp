#include "texture.hpp"

#include <cmath>
#include <fstream>
#include <string>

#include "nlohmann/json.hpp"
#include "raylib.h"

using json = nlohmann::json;

namespace tex {

void TextureManager::Load() {
      // clear old tex
      Unload();

      // open file + checks
      std::ifstream file(TextureManifestPath_);
      if (!file.is_open()) {
            TraceLog(LOG_WARNING, "[texture] could not open JSON file: %s",
                     TextureManifestPath_.c_str());
            return;
      }

      // Check empty file
      if (file.peek() == std::ifstream::traits_type::eof()) {
            TraceLog(LOG_WARNING, "[texture] JSON file is empty: %s",
                     TextureManifestPath_.c_str());
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
      if (!texturesJson.contains("textures") ||
          !texturesJson["textures"].is_array()) {
            TraceLog(LOG_WARNING,
                     "[texture] JSON missing valid \"texture array\": %s",
                     TextureManifestPath_.c_str());
            return;
      }

      // texture loading
      for (const auto& item : texturesJson["textures"]) {
            if (!item.contains("name") || !item.contains("filePath")) {
                  TraceLog(
                        LOG_WARNING,
                        "[texture] texture entry missing 'name' or 'filePath'");
                  continue;
            }

            std::string name = item["name"].get<std::string>();
            std::string path = item["filePath"].get<std::string>();

            Texture2D tex = LoadTexture(path.c_str());

            if (tex.id == 0) {
                  TraceLog(LOG_WARNING,
                           "[texture] failed to load texture at %s",
                           path.c_str());
            }

            loadedTexArray_.push_back({name, tex});
            TraceLog(LOG_INFO, "[texture] loaded %s from %s", name.c_str(),
                     path.c_str());
      }
}

[[nodiscard]] Texture2D* TextureManager::GetTexture(std::string& name) {
      Texture2D* tex;
      bool found = false;

      for (auto item : loadedTexArray_) {
            if (item.name == name) {
                  tex = &item.tex;
                  found = true;
                  break;
            }
      }

      if (!found) {
            tex = nullptr;
      }

      return tex;
}

void TextureManager::Unload() {
      for (const auto& item : loadedTexArray_) {
            UnloadTexture(item.tex);
      }
      loadedTexArray_.clear();
}

}  // namespace tex
