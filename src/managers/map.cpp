#include "map.hpp"
#include <optional>

#include "nlohmann/json.hpp"
#include "utilities.hpp"

using json = nlohmann::json;

namespace map {

// map stuff
int Map::LoadMap() {
      // parse json stuff
      json mapJSON{};
      if (!util::ParseJson(mapFilePath_, "map", mapJSON)) return -1;

      // do some checks on the JSON
      // make sure the root is an object
      if (!mapJSON.is_object()) return -2;
      // check required keys exist
      if (!mapJSON.contains("map_name") || !mapJSON.contains("layout")) return -3;
      // check map_name is a string
      if (!mapJSON["map_name"].is_string()) return -4;
      // check layout is an array
      if (!mapJSON["layout"].is_array()) return -5;

      mapName_ = mapJSON["map_name"].get<std::string>();

      // defining some vars
      const auto& layoutJSON = mapJSON["layout"];

      // check error
      if (layoutJSON.size() != MAP_HEIGHT) return -6;

      // reserve some space
      mapLayout_.clear();
      mapLayout_.reserve(MAP_HEIGHT);

      for (std::size_t rowIndex = 0; rowIndex < layoutJSON.size(); ++rowIndex) {
            const auto& rowJSON = layoutJSON[rowIndex];

            if (!rowJSON.is_array()) return -7;
            if (rowJSON.size() != MAP_WIDTH) return -8;

            std::vector<Tile> row;
            row.reserve(MAP_WIDTH);

            for (std::size_t colIndex = 0; colIndex < rowJSON.size(); ++colIndex) {
                const auto& tileJSON = rowJSON[colIndex];

                if (!tileJSON.is_number_integer()) return -9;

                int tileValue = tileJSON.get<int>();

                auto tileGroupOpt = TileGroupFromInt(tileValue);
                if (!tileGroupOpt.has_value()) return -10;

                raylib::Vector2 pos{
                    static_cast<float>(colIndex * TILE_SIZE),
                    static_cast<float>(rowIndex * TILE_SIZE)
                };

                row.emplace_back(*tileGroupOpt, pos);
            }

            mapLayout_.push_back(std::move(row));
      }

      return 0;
}



// helper stuff
std::optional<map::TileGroups> TileGroupFromInt(int value) {
    switch (value) {
        case 0: return map::TileGroups::Floor;
        case 1: return map::TileGroups::Counter;
        case 2: return map::TileGroups::Stove;
        case 3: return map::TileGroups::Fridge;
        default: return std::nullopt;
    }
}

}
