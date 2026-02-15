#pragma once
#include <SDL.h>
#include "core/Types.h"
#include "core/Config.h"
#include "db/Database.h"
#include "game/Menu.h"
#include "game/Game.h"

class App {
public:
    App();
    ~App();

    bool init();
    void run();

private:
    SDL_Window* win_ = nullptr;
    SDL_Renderer* ren_ = nullptr;

    Settings settings_;
    RuntimePaths paths_;
    Database db_;

    AppState state_ = AppState::Menu;

    Menu menu_;
    Game game_{settings_};

    int winW_ = 0;
    int winH_ = 0;

    void ensureDataDir();
    void loadSettings();
    void saveSettings();

    void handleEvent(const SDL_Event& e, bool& running);

    void toMenu();
};
