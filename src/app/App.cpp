#include "app/App.h"
#include "io/JsonMini.h"
#include "ui/Draw.h"

#include <iostream>
#include <filesystem>

App::App() {}
App::~App() {
    db_.shutdown();
    if (ren_) SDL_DestroyRenderer(ren_);
    if (win_) SDL_DestroyWindow(win_);
    SDL_Quit();
}

bool App::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        std::cerr << "SDL init failed: " << SDL_GetError() << "\n";
        return false;
    }

    ensureDataDir();
    loadSettings();

    winW_ = settings_.grid_w * settings_.cell_px;
    winH_ = settings_.grid_h * settings_.cell_px;

    win_ = SDL_CreateWindow("SnakePlus (C++)",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winW_, winH_, SDL_WINDOW_SHOWN);

    if (!win_) {
        std::cerr << "Window create failed\n";
        return false;
    }

    ren_ = SDL_CreateRenderer(win_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ren_) {
        std::cerr << "Renderer create failed\n";
        return false;
    }

    // DB init (optional)
    db_.init(paths_.scores_db);

    menu_.reset();
    game_.reset(settings_);
    return true;
}

void App::ensureDataDir() {
    std::error_code ec;
    std::filesystem::create_directories(paths_.data_dir, ec);
}

void App::loadSettings() {
    Settings s;
    if (JsonMini::loadSettings(paths_.settings_json, s)) {
        settings_ = s;
    } else {
        // create default file
        JsonMini::saveSettings(paths_.settings_json, settings_);
    }
}

void App::saveSettings() {
    JsonMini::saveSettings(paths_.settings_json, settings_);
}

void App::toMenu() {
    state_ = AppState::Menu;
    menu_.reset();
}

void App::handleEvent(const SDL_Event& e, bool& running) {
    if (e.type == SDL_QUIT) running = false;

    if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
        bool down = (e.type == SDL_KEYDOWN);
        int key = e.key.keysym.sym;

        if (state_ == AppState::Menu) {
            MenuResult res;
            menu_.onKey(key, down, settings_, db_, res);
            if (res.changed) saveSettings();

            if (res.next == AppState::Game) {
                state_ = AppState::Game;
                game_.reset(settings_);
            } else if (res.next == AppState::Quit) {
                running = false;
            }
        } else if (state_ == AppState::Game) {
            if (down && key == SDLK_RETURN) {
                // Enter => menu (when overlay shown)
                toMenu();
                return;
            }
            game_.onKey(key, down);
            if (down && key == SDLK_ESCAPE) {
                // ESC -> menu (fast)
                toMenu();
            }
        }
    }
}

void App::run() {
    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) handleEvent(e, running);

        if (state_ == AppState::Menu) {
            menu_.draw(ren_, winW_, winH_, settings_, db_);
        } else if (state_ == AppState::Game) {
            game_.update(SDL_GetTicks(), settings_, db_);
            game_.draw(ren_, winW_, winH_, settings_);
        }

        SDL_RenderPresent(ren_);
    }
}
