#pragma once
#include <forward_list>
#include "SDL2/SDL.h"
#include "Fighter.h"

class Champion;

/**
 * @brief Contains all information needed to run a game, like the Stage and all in-game Entities.
 */
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
    std::forward_list<Fighter> fighters; ///< A list containing all Fighters present in this Game.
    SDL_Rect camera;    ///< The area of the Stage that should be displayed.
    bool running;       ///Indicates if the game is actually running or not.
};