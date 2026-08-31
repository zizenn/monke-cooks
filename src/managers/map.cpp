#include "map.hpp"

#include <fstream>

#include "nlohmann/json.hpp"
#include "raylib.h"

using json = nlohmann::json;

int map::Map::LoadMap() {
      std::ifstream mapFile(mapFilePath_);
      if (!mapFile.is_open()) {
            TraceLog(LOG_ERROR, "[map] failed to open map file: %s",
                     mapFilePath_.c_str());
            return 1;
      }

      json mapData;

      try {
            mapFile >> mapData;
      } catch (const json::parse_error& e) {
            TraceLog(LOG_ERROR, "[map] JSON parse error in %s: %s",
                     mapFilePath_.c_str(), e.what());
            return 1;
      }

      return 0;
}
