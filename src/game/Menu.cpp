#include "game/Menu.h"
#include "ui/Draw.h"
#include <SDL.h>
#include <algorithm>

static SDL_Color C_BG{18,18,22,255};
static SDL_Color C_TEXT{235,238,245,255};
static SDL_Color C_MUTED{145,150,162,255};
static SDL_Color C_ACC{90,140,255,255};
static SDL_Color C_BAD{255,90,90,255};

void Menu::reset() {
    index_ = 0;
    view_ = AppState::Menu;
    settingsIndex_ = 0;
}

void Menu::onKey(int key, bool down, Settings& s, Database& db, MenuResult& out) {
    if (!down) return;

    out.changed = false;
    out.next = view_;

    auto up = [&](){ index_ = (index_ - 1 + 4) % 4; };
    auto dn = [&](){ index_ = (index_ + 1) % 4; };

    if (view_ == AppState::Menu) {
        if (key == SDLK_UP) up();
        if (key == SDLK_DOWN) dn();
        if (key == SDLK_RETURN || key == SDLK_KP_ENTER) {
            if (index_ == 0) out.next = AppState::Game;
            if (index_ == 1) view_ = AppState::Scores;
            if (index_ == 2) view_ = AppState::Settings;
            if (index_ == 3) out.next = AppState::Quit;
        }
    } else if (view_ == AppState::Scores) {
        if (key == SDLK_ESCAPE || key == SDLK_BACKSPACE) view_ = AppState::Menu;
    } else if (view_ == AppState::Settings) {
        // Settings items: speed_ms, wrap_walls, name
        const int items = 3;
        if (key == SDLK_UP) settingsIndex_ = (settingsIndex_ - 1 + items) % items;
        if (key == SDLK_DOWN) settingsIndex_ = (settingsIndex_ + 1) % items;

        if (key == SDLK_LEFT) {
            if (settingsIndex_ == 0) { s.speed_ms = std::max(50, s.speed_ms - 10); out.changed = true; }
            if (settingsIndex_ == 1) { s.wrap_walls = false; out.changed = true; }
        }
        if (key == SDLK_RIGHT) {
            if (settingsIndex_ == 0) { s.speed_ms = std::min(220, s.speed_ms + 10); out.changed = true; }
            if (settingsIndex_ == 1) { s.wrap_walls = true; out.changed = true; }
        }

        // Name edit (very simple): type A-Z, 0-9, space, backspace
        if (settingsIndex_ == 2) {
            if (key == SDLK_BACKSPACE) {
                if (!s.player_name.empty()) { s.player_name.pop_back(); out.changed = true; }
            } else {
                // We only handle a small set here (letters/digits/space)
                if ((key >= SDLK_a && key <= SDLK_z) || (key >= SDLK_0 && key <= SDLK_9) || key == SDLK_SPACE) {
                    char ch = (key == SDLK_SPACE) ? ' ' : (char)key;
                    if ((int)s.player_name.size() < 14) { s.player_name.push_back(ch); out.changed = true; }
                }
            }
        }

        if (key == SDLK_ESCAPE || key == SDLK_BACKSPACE) view_ = AppState::Menu;
    }
}

void Menu::draw(SDL_Renderer* r, int w, int h, const Settings& s, const Database& db) {
    Draw::rectFill(r, 0, 0, w, h, C_BG);

    if (view_ == AppState::Menu) drawMain(r, w, h);
    else if (view_ == AppState::Scores) drawScores(r, w, h, db);
    else if (view_ == AppState::Settings) drawSettings(r, w, h, s);
}

void Menu::drawMain(SDL_Renderer* r, int w, int h) {
    Draw::text(r, w/2 - 110, 60, "SNAKEPLUS", C_ACC, 4);
    Draw::text(r, w/2 - 150, 110, "ARROWS TO MOVE - ENTER TO SELECT", C_MUTED, 2);

    const char* items[4] = { "PLAY", "HIGH SCORES", "SETTINGS", "QUIT" };
    int y = 190;
    for (int i = 0; i < 4; i++) {
        SDL_Color c = (i == index_) ? C_TEXT : C_MUTED;
        if (i == index_) {
            Draw::rectFill(r, w/2 - 170, y - 8, 340, 32, SDL_Color{32,34,42,255});
            Draw::rectOutline(r, w/2 - 170, y - 8, 340, 32, SDL_Color{60,70,92,255});
        }
        Draw::text(r, w/2 - 60, y, items[i], c, 3);
        y += 54;
    }
}

void Menu::drawScores(SDL_Renderer* r, int w, int h, const Database& db) {
    Draw::text(r, w/2 - 140, 60, "HIGH SCORES", C_ACC, 4);
    Draw::text(r, w/2 - 120, 110, "ESC/BACKSPACE TO RETURN", C_MUTED, 2);

    int y = 170;
    if (!db.enabled()) {
        Draw::text(r, w/2 - 170, y, "DB DISABLED (SQLITE NOT FOUND)", C_BAD, 2);
        return;
    }
    auto rows = db.topScores(8);
    if (rows.empty()) {
        Draw::text(r, w/2 - 60, y, "NO SCORES YET", C_MUTED, 3);
        return;
    }
    int i = 1;
    for (auto& row : rows) {
        Draw::text(r, w/2 - 220, y, std::to_string(i) + ".", C_MUTED, 2);
        Draw::text(r, w/2 - 180, y, row.name, C_TEXT, 2);
        Draw::text(r, w/2 + 40, y, std::to_string(row.score), C_ACC, 2);
        y += 36;
        i++;
    }
}

void Menu::drawSettings(SDL_Renderer* r, int w, int h, const Settings& s) {
    Draw::text(r, w/2 - 120, 60, "SETTINGS", C_ACC, 4);
    Draw::text(r, w/2 - 120, 110, "LEFT/RIGHT TO CHANGE", C_MUTED, 2);
    Draw::text(r, w/2 - 120, 132, "ESC TO RETURN", C_MUTED, 2);

    int y = 190;

    auto item = [&](int idx, const std::string& label, const std::string& value) {
        bool sel = (idx == settingsIndex_);
        if (sel) {
            Draw::rectFill(r, w/2 - 240, y - 8, 480, 34, SDL_Color{32,34,42,255});
            Draw::rectOutline(r, w/2 - 240, y - 8, 480, 34, SDL_Color{60,70,92,255});
        }
        Draw::text(r, w/2 - 220, y, label, sel ? C_TEXT : C_MUTED, 2);
        Draw::text(r, w/2 + 80, y, value, sel ? C_ACC : C_MUTED, 2);
        y += 54;
    };

    item(0, "SPEED_MS", std::to_string(s.speed_ms));
    item(1, "WRAP_WALLS", s.wrap_walls ? "TRUE" : "FALSE");
    item(2, "NAME", s.player_name.empty() ? "PLAYER" : s.player_name);
}
