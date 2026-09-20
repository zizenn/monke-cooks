#pragma once

#include <string>
#include <vector>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace lvl {

const std::string LevelManifestPath = "gameData/levels/levels.json";

struct LevelContainer {
      int Id;
      std::string Name;
      std::string MapPath;
      std::string TilePath;
      std::string TexPath;
};

class Level {
private:
      // variables
      std::vector<LevelContainer> levels_{};

      // functions
      void Initialize();
      void LoadLevel(int id);

public:
};

}  // namespace lvl
