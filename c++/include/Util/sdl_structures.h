#include "structures.h"
#include "SDL2/SDL_rect.h"

template <typename T>
SDL_Rect rect_to_sdl_rect(const Kuribrawl::Rect<T>& rect){
    return SDL_Rect{rect.x, rect.y, rect.w, rect.h};
}

template <typename T>
SDL_Rect rect_borders_to_sdl_rect(const Kuribrawl::Rect<T>& rect){
    return SDL_Rect{rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top};
}

template <typename T>
SDL_Rect rect_borders_vreverse_to_sdl_rect(const Kuribrawl::Rect<T>& rect){
    return SDL_Rect{rect.left, rect.top, rect.right - rect.left, rect.top - rect.bottom};
}
