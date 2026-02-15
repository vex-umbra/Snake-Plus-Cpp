#pragma once
#include <vector>
#include <string>
#include "core/Types.h"
#include "core/Config.h"
#include "db/Database.h"

struct MenuResult {
    AppState next = AppState::Menu;
    bool changed = false;
};

class Menu {
public:
    void reset();

    // keyboard input
    void onKey(int key, bool down, Settings& s, Database& db, MenuResult& out);

    // render
    void draw(class SDL_Renderer* r, int w, int h, const Settings& s, const Database& db);

private:
    int index_ = 0;
    AppState view_ = AppState::Menu;

    // for settings editing
    int settingsIndex_ = 0;

    void drawMain(class SDL_Renderer* r, int w, int h);
    void drawScores(class SDL_Renderer* r, int w, int h, const Database& db);
    void drawSettings(class SDL_Renderer* r, int w, int h, const Settings& s);
};
