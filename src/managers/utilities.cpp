#include "utilities.hpp"

#include <fstream>
#include <istream>
#include <string>

#include "nlohmann/json.hpp"
#include "raylib.h"

using json = nlohmann::json;

namespace util {

bool ParseJson(const std::string& path, const std::string& label,
               json& object) {
      object = nullptr;

      // open file
      std::ifstream file(path);
      if (!file.is_open()) {
            TraceLog(LOG_WARNING, "[%s] could not open JSON file: %s",
                     label.c_str(), path.c_str());
            return false;
      }

      // check empty file
      file >> std::ws;
      if (file.peek() == std::ifstream::traits_type::eof()) {
            TraceLog(LOG_WARNING,
                     "[%s] JSON file is empty or only whitespace: %s",
                     label.c_str(), path.c_str());
            return false;
      }

      // transfer json into code
      try {
            file >> object;
            TraceLog(LOG_INFO, "[%s] successfully parsed JSON into c++",
                     label.c_str());
            return true;
      } catch (const json::parse_error& e) {
            TraceLog(LOG_WARNING, "[%s] JSON parse error in %s: %s",
                     label.c_str(), path.c_str(), e.what());
            return false;
      }
}

}  // namespace util
