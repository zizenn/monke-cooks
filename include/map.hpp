#pragma once

#include <sys/types.h>

#include <string>

#include "raylib.h"

namespace map {

// constants
static const int TILE_SIZE =
      32;  // 32 means 32x32 image sizes for the texture, if the texture is
           // greater, then this number it will be scaled to fit

// enums
enum class TileGroups {
      Floor,
      Counter,
};

// tile class
class Tile {
private:
      TileGroups tileGroup_;
};

// map class
class Map {
private:
      std::string mapFilePath_;
      const Vector2 mapSize_ = {16, 9};
};

}  // namespace map
