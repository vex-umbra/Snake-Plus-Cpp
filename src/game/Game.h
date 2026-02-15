#pragma once
#include <SDL.h>
#include "core/Config.h"
#include "core/Timer.h"
#include "game/Snake.h"
#include "game/World.h"
#include "db/Database.h"

class Game {
public:
    explicit Game(const Settings& s);

    void reset(const Settings& s);
    void setSettings(const Settings& s);

    // returns next state (Menu when user exits)
    AppState update(uint32_t nowMs, const Settings& s, Database& db);
    void onKey(int key, bool down);

    void draw(SDL_Renderer* r, int w, int h, const Settings& s);

private:
    Settings s_;
    FixedTimer timer_;
    Snake snake_;
    World world_;
    int score_ = 0;
    bool gameOver_ = false;

    // input direction
    Vec2i desired_{1,0};

    void newRound();
};
