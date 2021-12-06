#include "Text/TextureFont.h"
#include <string>

TextureFont::TextureFont(SDL_Texture* sheet_, SDL_Renderer* renderer_, const Kuribrawl::Rect& char_size_):
    char_sheet(sheet_),
    char_size(char_size_),
    renderer(renderer_)
{
    if (sheet_ == nullptr) throw;
    int w; SDL_QueryTexture(sheet_, NULL, NULL, &w, NULL);
    chars_per_line = w / char_size.w;
}

TextureFont::TextureFont(const char* filename, SDL_Renderer* renderer, const Kuribrawl::Rect& char_size) : 
    TextureFont(IMG_LoadTexture(renderer, filename), renderer, char_size)
{
}

void TextureFont::getCharPos(char c, SDL_Rect& result) const{
    result.x = ((int)c % chars_per_line) * char_size.w;
    result.y = ((int)c / chars_per_line) * char_size.h;
}

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

void TextureFont::displayString(const std::string& s, int& x, int& y, SDL_Renderer* target) const{
    target = (target) ? target : renderer;
    for (char c : s){
        displayChar(c, x, y, target);
        x += char_size.w;
    }
}