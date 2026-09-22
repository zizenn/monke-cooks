#include "map.hpp"

#include "nlohmann/json.hpp"
#include "utilities.hpp"

using json = nlohmann::json;

int map::Map::LoadMap() {
      // parse json stuff
      json mapJSON{};
      if (!util::ParseJson(mapFilePath_, "map", mapJSON)) return -1;

      // do some checks on the JSON

      return 0;
}
