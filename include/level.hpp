#pragma once

#include <string>
#include <vector>
namespace lvl {

class Level {
private:
      const std::string LEVEL_FILE_PATH_;
      std::string MAP_FILE_PATH_;
      std::vector<std::string> texPaths_;

public:
      Level(std::string lvlPath) : MAP_FILE_PATH_(lvlPath) {}
      bool LoadJSON();
};

}
