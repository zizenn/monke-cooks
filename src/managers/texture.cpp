#include "texture.hpp"

#include <string>

#include "nlohmann/json.hpp"
#include "raylib.h"
#include "utilities.hpp"

using json = nlohmann::json;

namespace tex {

void TextureManager::Load() {
      // clear old tex
      Unload();

      json texturesJson{};

      if (!util::ParseJson(TextureManifestPath_, "texture", texturesJson)) {
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
