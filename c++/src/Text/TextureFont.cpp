#include "Text/TextureFont.h"
#include <string>

/**
 * @brief Construct a new Texture Font from an image and its renderer.
 * 
 * @param sheet_ 
 * @param renderer_ 
 * @param char_size_ Size of glyphs on the image
 */
TextureFont::TextureFont(SDL_Texture* sheet_, SDL_Renderer* renderer_, const Kuribrawl::Size& char_size_):
    char_sheet(sheet_),
    char_size(char_size_),
    renderer(renderer_)
{
    if (sheet_ == nullptr) throw;
    int w; SDL_QueryTexture(sheet_, NULL, NULL, &w, NULL);
    chars_per_line = w / char_size.w;
}

/**
 * @brief Construct a new Texture Font from a filename, attempting to load the image from this file.
 * 
 * @param filename 
 * @param renderer Renderer that will be used to load the image.
 * @param char_size Size of glyphs on the image
 */
TextureFont::TextureFont(const char* filename, SDL_Renderer* renderer, const Kuribrawl::Size& char_size) : 
    TextureFont(IMG_LoadTexture(renderer, filename), renderer, char_size)
{
}

/**
 * @brief Exposes the position of the glyph for a given char.
 * 
 * Actually modifies a rectangle structure instead of a point for optimization reasons.
 * 
 * @param c 
 * @param result Rectangle structure that will be updated with the position (if the size of the rectangle is the char_size, it can be used to render right away) 
 */
void TextureFont::getCharPos(char c, SDL_Rect& result) const{
    result.x = ((int)c % chars_per_line) * char_size.w;
    result.y = ((int)c / chars_per_line) * char_size.h;
}

/**
 * @brief Displays a character.
 * 
 * @param c 
 * @param x The position on the render target at wich the string will be displayed.
 * @param y 
 * @param target The rendering target.
 */
inline void TextureFont::displayChar(char c, int x, int y, SDL_Renderer* target) const {
    SDL_Rect src;
    getCharPos(c, src);
    src.w = char_size.w;
    src.h = char_size.h;
    SDL_Rect dst = {x, y};
    dst.w = char_size.w;
    dst.h = char_size.h;
    SDL_RenderCopy((target) ? target : renderer, char_sheet, &src, &dst);
}

/**
 * @brief Displays a string.
 * 
 * @param s
 * @param x The position on the render target at wich the string will be displayed.
 * @param y 
 * @param target The rendering target.
 */
void TextureFont::displayString(const std::string& s, int& x, int& y, SDL_Renderer* target) const{
    target = (target) ? target : renderer;
    for (char c : s){
        displayChar(c, x, y, target);
        x += char_size.w;
    }
}

/**
 * @brief Returns wheter this texture font is ready to use.  
 * Cases where this returns false are usually error cases.  
 * @return bool
 */
bool TextureFont::isInitialized() const {
    return char_sheet != nullptr; 
}