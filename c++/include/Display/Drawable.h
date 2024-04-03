#pragma once

class SDL_Renderer;
class Camera;

class Drawable {
    public:
    virtual void draw(SDL_Renderer* target, const Camera& cam) const = 0;
};