#pragma once

#include <string>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace lvl {

class Level {
private:
      const std::string LevelFilePath_;  // given at start
      const std::string MapFilePath_;
      const std::string TextureManifestPath_;

      // functions

public:
      Level(std::string lvlPath) : LevelFilePath_(lvlPath) {}
};

}  // namespace lvl
