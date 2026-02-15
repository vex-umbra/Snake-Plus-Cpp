#include "game/World.h"
#include <SDL.h>

World::World(const Settings& s) : s_(s), rng_((unsigned)SDL_GetTicks()) {
    reset(s);
}

void World::setSettings(const Settings& s) {
    s_ = s;
}

void World::reset(const Settings& s) {
    s_ = s;
    // food placed later by respawn
}

const Vec2i& World::food() const { return food_; }

bool World::inside(const Vec2i& p) const {
    return p.x >= 0 && p.x < s_.grid_w && p.y >= 0 && p.y < s_.grid_h;
}

void World::respawnFood(const Snake& snake) {
    std::uniform_int_distribution<int> rx(0, s_.grid_w - 1);
    std::uniform_int_distribution<int> ry(0, s_.grid_h - 1);
    while (true) {
        Vec2i f{ rx(rng_), ry(rng_) };
        if (!snake.occupies(f)) { food_ = f; return; }
    }
}

bool World::update(Snake& snake, bool& outGameOver, int& inOutScore) {
    outGameOver = false;

    Vec2i head = snake.body().front();
    Vec2i next{ head.x + snake.dir().x, head.y + snake.dir().y };

    // wrap or wall
    if (s_.wrap_walls) {
        if (next.x < 0) next.x = s_.grid_w - 1;
        if (next.x >= s_.grid_w) next.x = 0;
        if (next.y < 0) next.y = s_.grid_h - 1;
        if (next.y >= s_.grid_h) next.y = 0;
    } else {
        if (!inside(next)) { outGameOver = true; return false; }
    }

    // self collision check against body (current)
    if (snake.hitsSelf(next)) { outGameOver = true; return false; }

    bool ate = (next == food_);
    snake.step(ate);

    if (ate) {
        inOutScore += 10;
        respawnFood(snake);
    }

    return ate;
}
