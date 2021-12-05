#pragma once

#include <memory>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "Util/util.h"

class TextureFont {
    public:
    SDL_Texture* char_sheet;
    Kuribrawl::Rect char_size;
    int chars_per_line;
    SDL_Renderer* renderer;
    bool is_init;

    void displayChar(char c, int x, int y, SDL_Renderer* target) const;

    public:
    TextureFont(SDL_Texture* sheet, SDL_Renderer* renderer, const Kuribrawl::Rect& char_size);
    TextureFont(const char* filename, SDL_Renderer* renderer, const Kuribrawl::Rect& char_size);

    void getCharPos(char c, SDL_Rect& result) const;

    void displayString(const std::string& s, int& x, int& y, SDL_Renderer* target = NULL) const;

    bool isInitialized();
};