#pragma once
#include <string>
#include "core/Config.h"

// Tiny JSON reader/writer for a *small* subset we need.
// Supports: { "key": int/bool/string, ... }
namespace JsonMini {
    bool loadSettings(const std::string& path, Settings& out);
    bool saveSettings(const std::string& path, const Settings& s);
}
