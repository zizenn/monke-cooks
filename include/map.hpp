#pragma once

#include <sys/types.h>

#include <string>

#include "Vector2.hpp"

namespace map {

// constants
static const int TILE_SIZE =
      32;  // 32 means 32x32 image sizes for the texture, if the texture is
           // greater, then this number it will be scaled to fit

// enums
enum class TileGroups {
      Floor,    // 0
      Counter,  // 1
      Stove,    // 2
      Fridge,   // 3
};

// tile class
class Tile {
private:
      TileGroups tileGroup_;
      raylib::Vector2 pos;
};

// map class
class Map {
private:
      std::string mapFilePath_;
      const raylib::Vector2 mapSize_ = {16, 9};

public:
      int LoadMap();
};

}  // namespace map
