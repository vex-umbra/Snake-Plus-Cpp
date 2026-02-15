#pragma once
#include <SDL.h>
#include <string>

namespace Draw {
    void rectFill(SDL_Renderer* r, int x, int y, int w, int h, SDL_Color c);
    void rectOutline(SDL_Renderer* r, int x, int y, int w, int h, SDL_Color c);

    // Very tiny pixel-font-ish text (no TTF libs). Not pretty, but works for menus.
    // Supports A-Z, 0-9, space, :, -, .
    void text(SDL_Renderer* r, int x, int y, const std::string& s, SDL_Color c, int scale=2);
}
