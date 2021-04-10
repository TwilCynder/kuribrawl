#pragma once
#include <forward_list>
#include "SDL2/SDL.h"
#include "Fighter.h"
#include "Champion.h"

class Game {
    public:
    Game();
    Fighter* addFighter(Champion* model);
    Fighter* addFighter(Champion* model, int x, int y);
    bool is_running();

    //main loop

    void applyPhysics();
    void advanceAnimations();
    void draw(SDL_Renderer* target);
    void updateInputs();
    void updateStates();
    void updateAnimations();

    private:
    std::forward_list<Fighter> fighters;
    SDL_Rect camera;
    bool running;
};