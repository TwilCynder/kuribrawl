#pragma once

#include <memory>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "Util/util.h"

/**
 * @brief Bitmap font displayable on a GPU renderer.
 * 
 */
class TextureFont {
    public:
    SDL_Texture* char_sheet;    /**<Image used as the source, containing all the glyphs/visuals for characters, organized in a table-like structure*/
    Kuribrawl::Size char_size;  /**<Size of glyphs on the source image*/
    int chars_per_line;         /**<Number of characters on each line*/
    SDL_Renderer* renderer;     /**<Renderer the source image is bound to.*/
    bool is_init;

    void displayChar(char c, int x, int y, SDL_Renderer* target) const;

    public:
    TextureFont(SDL_Texture* sheet, SDL_Renderer* renderer, const Kuribrawl::Size& char_size);
    TextureFont(const char* filename, SDL_Renderer* renderer, const Kuribrawl::Size& char_size);

    void getCharPos(char c, SDL_Rect& result) const;

    void displayString(const std::string& s, int x, int y, SDL_Renderer* target = NULL) const;
    void displayStringRef(const std::string& s, int& x, int& y, SDL_Renderer* target = NULL) const;
    void displayStringCentered(const std::string& s, int x, int y, SDL_Renderer* target = NULL) const;

    bool isInitialized() const;
};