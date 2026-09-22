#include "level.hpp"

#include "raylib.h"
#include "utilities.hpp"

namespace lvl {

void Level::Initialize() {
      // parse the json
      json levels;

      if (!util::ParseJson(LevelManifestPath, "level", levels)) {
            return;
      }

      // -- extract data --

      // create temp struct for lvl filling
      LevelContainer lvl;

      for (const auto& item : levels["levels"]) {
            // error checks
            if (!item.contains("id") || !item.contains("name") ||
                !item.contains("mapPath") || !item.contains("tilePath") ||
                !item.contains("texPath")) {
                  TraceLog(LOG_WARNING, "[level] incomplete data set at %s",
                           LevelManifestPath.c_str());
                  continue;
            }

            lvl.Id = item["id"].get<int>();
            lvl.Name = item["name"].get<std::string>();
            lvl.MapPath = item["mapPath"].get<std::string>();
            lvl.TilePath = item["tilePath"].get<std::string>();
            lvl.TexPath = item["texPath"].get<std::string>();

            // add lvl to vector
            levels_.push_back(lvl);

            // clear lvl
            lvl = {};
      }
}

void Level::LoadLevel(int id) {
      for (const auto& item : levels_) {
            if (item.Id == id) {
                  Load_(item);
                  break;
            }
            continue;
      }
}

void Level::Load_(const LevelContainer& levelToLoad) {
      texMan_.Set(levelToLoad.TexPath);
      texMan_.Load();
      // load tile
      // load map
      TraceLog(LOG_INFO, "[level] level %d-%s loaded", levelToLoad.Id,
               levelToLoad.Name.c_str());
}
}  // namespace lvl
