#pragma once
#include <forward_list>
#include "SDL2/SDL.h"
#include "PlayerFighter.h"

class Champion;

/**
 * @brief Contains all information needed to run a game, like the Stage and all in-game Entities.
 */
class Game {
    public:
    Game();
    PlayerFighter* addFighter(Champion* model);
    PlayerFighter* addFighter(Champion* model, int x, int y);
    bool is_running();

    //main loop

    void step(SDL_Renderer* render_target);

    void applyPhysics();
    void advanceAnimations();
    void draw(SDL_Renderer* target);
    void updateInputs();
    void updateStates();
    void updateAnimations();

    int getFrame();

    private:
    int frame;
    std::forward_list<PlayerFighter> fighters; ///< A list containing all Fighters present in this Game.
    using Fighteriterator = std::forward_list<PlayerFighter>::iterator;
    SDL_Rect camera;    ///< The area of the Stage that should be displayed.
    bool running;       ///Indicates if the game is actually running or not.
};