#include "game/Snake.h"

void Snake::reset(int x, int y) {
    body_.clear();
    body_.push_back({x, y});
    body_.push_back({x - 1, y});
    body_.push_back({x - 2, y});
    dir_ = {1, 0};
    pending_ = dir_;
}

void Snake::setDir(const Vec2i& d) {
    // avoid instant reverse
    if (d.x == -dir_.x && d.y == -dir_.y) return;
    pending_ = d;
}

const Vec2i& Snake::dir() const { return dir_; }

Vec2i Snake::step(bool grow) {
    dir_ = pending_;
    Vec2i head = body_.front();
    Vec2i next{ head.x + dir_.x, head.y + dir_.y };
    body_.push_front(next);
    if (!grow) body_.pop_back();
    return next;
}

bool Snake::hitsSelf(const Vec2i& p) const {
    for (auto& s : body_) if (s == p) return true;
    return false;
}

bool Snake::occupies(const Vec2i& p) const {
    for (auto& s : body_) if (s == p) return true;
    return false;
}

const std::deque<Vec2i>& Snake::body() const { return body_; }
