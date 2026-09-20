#pragma once

#include <string>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace util {

bool ParseJson(const std::string& path, const std::string& label, json& object);

}  // namespace util
