#pragma once

#include <optional>

#include <string>
#include <vector>

#include "Vector2.hpp"

namespace map {

// constants
constexpr std::size_t MAP_WIDTH = 16;
constexpr std::size_t MAP_HEIGHT = 9;
constexpr int TILE_SIZE =
      32;  // 32 means 32x32 image sizes for the texture, if the texture is
           // greater than this number, it will be scaled to fit

// enums
enum class TileGroups {
      Floor = 0,
      Counter = 1,
      Stove = 2,
      Fridge = 3,
};

// tile class
class Tile {
private:
      TileGroups tileGroup_;
      raylib::Vector2 pos_;
public:
      // constructor
      Tile(TileGroups tileGroup, const raylib::Vector2& pos)
            : tileGroup_(tileGroup), pos_(pos) {}

      // functions
      TileGroups GetGroup() const& { return tileGroup_; }
      raylib::Vector2 GetPos() const& { return pos_; }
};

// map class
class Map {
private:
      // constants
      const std::string mapFilePath_;

      // variables
      std::string mapName_;
      std::vector<std::vector<Tile>> mapLayout_;

      // functions
      int LoadTiles_();
public:
      // constructor
      Map(const std::string& mapFilePath) : mapFilePath_(mapFilePath) {}

      // methods
      int LoadMap();

      // getters
      const std::string& GetName() const { return mapName_; }
      const std::vector<std::vector<Tile>>& GetLayout() const { return mapLayout_; }
};

// helper functions
std::optional<map::TileGroups> TileGroupFromInt(int value);

}  // namespace map
