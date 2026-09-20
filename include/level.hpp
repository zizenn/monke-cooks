#pragma once

#include <string>
namespace lvl {

class Level {
private:
      const std::string LevelFilePath_;  // given at start
      const std::string MapFilePath_;
      const std::string TextureManifestPath_;

      // functions
      void ParseJSON();

public:
      Level(std::string lvlPath) : LevelFilePath_(lvlPath) {}
};

}  // namespace lvl
