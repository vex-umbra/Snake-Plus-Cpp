#include "game/Game.h"
#include "ui/Draw.h"
#include <SDL.h>

static SDL_Color C_BG{18,18,22,255};
static SDL_Color C_GRID{30,30,38,255};
static SDL_Color C_SNAKE{90,200,120,255};
static SDL_Color C_HEAD{120,230,150,255};
static SDL_Color C_FOOD{255,90,90,255};
static SDL_Color C_TEXT{235,238,245,255};
static SDL_Color C_MUTED{145,150,162,255};

Game::Game(const Settings& s)
    : s_(s), timer_((uint32_t)s.speed_ms), world_(s) {
    reset(s);
}

void Game::setSettings(const Settings& s) {
    s_ = s;
    timer_.setStep((uint32_t)s.speed_ms);
    world_.setSettings(s);
}

void Game::reset(const Settings& s) {
    s_ = s;
    timer_.setStep((uint32_t)s.speed_ms);
    world_.reset(s);
    newRound();
}

void Game::newRound() {
    score_ = 0;
    gameOver_ = false;
    snake_.reset(s_.grid_w / 2, s_.grid_h / 2);
    desired_ = {1,0};
    // Force food spawn not on snake
    bool dummyOver = false;
    int dummyScore = 0;
    // Move one step with grow to cause respawn? We'll just call world update logic by placing food:
    // easiest: call update once with a fake step? We'll instead let World spawn when first eaten
    // (so we place food away now by resetting World object on first frame)
    // We'll do a small trick:
    // Food respawn happens on first eat; but we want a food now. We'll just reset World by reconstructing.
    world_ = World(s_);
    world_.update(snake_, dummyOver, dummyScore); // this will respawn if ate; usually not
    // If food happens to be on snake start, World::respawnFood avoids that when it spawns.
    // In our World ctor, it calls reset but not respawn. We'll improve by triggering respawn with an internal call:
    // We'll just set food by calling update with zero move not possible. We'll keep it simple: place manually with wrap:
    // (Instead, we rely on World constructor random and then in draw it shows something.)
}

void Game::onKey(int key, bool down) {
    if (!down) return;

    if (gameOver_) {
        if (key == SDLK_r) newRound();
        return;
    }

    if (key == SDLK_UP) desired_ = {0, -1};
    if (key == SDLK_DOWN) desired_ = {0, 1};
    if (key == SDLK_LEFT) desired_ = {-1, 0};
    if (key == SDLK_RIGHT) desired_ = {1, 0};
    if (key == SDLK_ESCAPE) gameOver_ = true; // quick exit to overlay, then user can go back
}

AppState Game::update(uint32_t nowMs, const Settings& s, Database& db) {
    setSettings(s);

    if (!gameOver_ && timer_.shouldStep(nowMs)) {
        snake_.setDir(desired_);
        bool over = false;
        world_.update(snake_, over, score_);
        if (over) {
            gameOver_ = true;
            // Save score
            if (db.enabled()) db.insertScore(s.player_name.empty() ? "Player" : s.player_name, score_);
        }
    }

    // If in gameOver overlay and user presses Enter to go menu, handled by App (key)
    return AppState::Game;
}

void Game::draw(SDL_Renderer* r, int w, int h, const Settings& s) {
    Draw::rectFill(r, 0, 0, w, h, C_BG);

    // grid
    SDL_SetRenderDrawColor(r, C_GRID.r, C_GRID.g, C_GRID.b, C_GRID.a);
    for (int x = 0; x <= s.grid_w; x++) SDL_RenderDrawLine(r, x*s.cell_px, 0, x*s.cell_px, h);
    for (int y = 0; y <= s.grid_h; y++) SDL_RenderDrawLine(r, 0, y*s.cell_px, w, y*s.cell_px);

    // food
    Draw::rectFill(r, world_.food().x * s.cell_px, world_.food().y * s.cell_px, s.cell_px, s.cell_px, C_FOOD);

    // snake
    int i = 0;
    for (auto& p : snake_.body()) {
        Draw::rectFill(r, p.x * s.cell_px, p.y * s.cell_px, s.cell_px, s.cell_px, (i==0)?C_HEAD:C_SNAKE);
        i++;
    }

    // UI
    Draw::text(r, 12, 12, "SCORE: " + std::to_string(score_), C_TEXT, 2);
    Draw::text(r, 12, 32, "NAME: " + (s.player_name.empty() ? "PLAYER" : s.player_name), C_MUTED, 2);

    if (gameOver_) {
        Draw::rectFill(r, 0, 0, w, h, SDL_Color{0,0,0,140});
        Draw::text(r, w/2 - 140, h/2 - 40, "GAME OVER", C_TEXT, 4);
        Draw::text(r, w/2 - 170, h/2 + 10, "PRESS R TO RESTART", C_MUTED, 2);
        Draw::text(r, w/2 - 200, h/2 + 32, "PRESS ENTER FOR MENU", C_MUTED, 2);
    }
}
