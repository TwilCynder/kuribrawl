#pragma once

class SDL_Renderer;

class Drawable {
    public:
    virtual void draw(SDL_Renderer* target) const = 0;
};