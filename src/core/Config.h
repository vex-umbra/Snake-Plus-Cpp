#pragma once
#include <string>

struct Settings {
    int speed_ms = 110;      // lower = faster
    bool wrap_walls = false; // if true, snake wraps around edges
    int grid_w = 32;
    int grid_h = 24;
    int cell_px = 20;

    std::string player_name = "Player";
};

struct RuntimePaths {
    std::string data_dir = "data";
    std::string settings_json = "data/settings.json";
    std::string scores_db = "data/scores.db";
};
