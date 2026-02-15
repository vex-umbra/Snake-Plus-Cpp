#include "io/JsonMini.h"
#include <fstream>
#include <sstream>
#include <algorithm>

static std::string readAll(const std::string& path) {
    std::ifstream f(path);
    if (!f) return {};
    std::ostringstream ss; ss << f.rdbuf();
    return ss.str();
}

static void writeAll(const std::string& path, const std::string& s) {
    std::ofstream f(path);
    f << s;
}

static std::string trim(std::string s) {
    auto notSpace = [](unsigned char c){ return !std::isspace(c); };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), notSpace));
    s.erase(std::find_if(s.rbegin(), s.rend(), notSpace).base(), s.end());
    return s;
}

static bool findInt(const std::string& json, const std::string& key, int& out) {
    auto pos = json.find('"' + key + '"');
    if (pos == std::string::npos) return false;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return false;
    pos++;
    auto end = json.find_first_of(",}", pos);
    if (end == std::string::npos) return false;
    std::string v = trim(json.substr(pos, end - pos));
    try { out = std::stoi(v); return true; } catch (...) { return false; }
}

static bool findBool(const std::string& json, const std::string& key, bool& out) {
    auto pos = json.find('"' + key + '"');
    if (pos == std::string::npos) return false;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return false;
    pos++;
    auto end = json.find_first_of(",}", pos);
    if (end == std::string::npos) return false;
    std::string v = trim(json.substr(pos, end - pos));
    if (v == "true") { out = true; return true; }
    if (v == "false") { out = false; return true; }
    return false;
}

static bool findString(const std::string& json, const std::string& key, std::string& out) {
    auto pos = json.find('"' + key + '"');
    if (pos == std::string::npos) return false;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return false;
    pos++;
    auto q1 = json.find('"', pos);
    if (q1 == std::string::npos) return false;
    auto q2 = json.find('"', q1 + 1);
    if (q2 == std::string::npos) return false;
    out = json.substr(q1 + 1, q2 - (q1 + 1));
    return true;
}

namespace JsonMini {
    bool loadSettings(const std::string& path, Settings& out) {
        std::string j = readAll(path);
        if (j.empty()) return false;

        findInt(j, "speed_ms", out.speed_ms);
        findBool(j, "wrap_walls", out.wrap_walls);
        findInt(j, "grid_w", out.grid_w);
        findInt(j, "grid_h", out.grid_h);
        findInt(j, "cell_px", out.cell_px);
        findString(j, "player_name", out.player_name);
        return true;
    }

    bool saveSettings(const std::string& path, const Settings& s) {
        std::ostringstream o;
        o << "{\n";
        o << "  \"speed_ms\": " << s.speed_ms << ",\n";
        o << "  \"wrap_walls\": " << (s.wrap_walls ? "true" : "false") << ",\n";
        o << "  \"grid_w\": " << s.grid_w << ",\n";
        o << "  \"grid_h\": " << s.grid_h << ",\n";
        o << "  \"cell_px\": " << s.cell_px << ",\n";
        o << "  \"player_name\": \"" << s.player_name << "\"\n";
        o << "}\n";
        writeAll(path, o.str());
        return true;
    }
}
