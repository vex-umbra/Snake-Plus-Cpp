#pragma once
#include <random>
#include "core/Types.h"
#include "core/Config.h"
#include "game/Snake.h"

class World {
public:
    explicit World(const Settings& s);

    void reset(const Settings& s);
    void setSettings(const Settings& s);

    const Vec2i& food() const;
    bool inside(const Vec2i& p) const;

    // returns true if food eaten
    bool update(Snake& snake, bool& outGameOver, int& inOutScore);

private:
    Settings s_;
    Vec2i food_{5,5};

    std::mt19937 rng_;
    void respawnFood(const Snake& snake);
};
