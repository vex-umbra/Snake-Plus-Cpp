#pragma once
#include <deque>
#include "core/Types.h"

class Snake {
public:
    void reset(int startX, int startY);
    void setDir(const Vec2i& d);
    const Vec2i& dir() const;

    // Move one step; returns new head position
    Vec2i step(bool grow);

    bool hitsSelf(const Vec2i& p) const;
    bool occupies(const Vec2i& p) const;

    const std::deque<Vec2i>& body() const;

private:
    std::deque<Vec2i> body_;
    Vec2i dir_{1, 0};
    Vec2i pending_{1, 0};
};
