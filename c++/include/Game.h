#pragma once
#include <forward_list>
#include <memory>
#include "SDL2/SDL.h"
#include "PlayerFighter.h"

class Champion;
class StageModel;
class Stage;
class TextureFont;
class GameConfiguration;

using GameConfSPtr = std::shared_ptr<GameConfiguration>;

/**
 * @brief Contains all information needed to run a game, like the Stage and all in-game Entities.
 */
class Game {
    public:

    Game();
    Game(GameConfiguration&);
    Game(GameConfSPtr&);
    ~Game();
    void applyConfig(GameConfiguration&);
    PlayerFighter* addFighter(Champion* model);
    PlayerFighter* addFighter(Champion* model, int x, int y);
    PlayerFighter* addFighter(Champion* model, int x, int y, Port& port);
    bool is_running();

    //main loop

    void step(SDL_Renderer* render_target);

    void applyPhysics();
    void advanceAnimations();
    void draw(SDL_Renderer* target);
    void updateInputsState();
    void resolveInputs();
    void updateStates();
    void updateAnimations();
    void hitDetection();

    //Debug
    void drawDebugInfo(TextureFont& font, SDL_Renderer* target);
    int getFrame();

    struct Result {
        bool completed;     ///<Indicates whether the game was completed; if false, most of the values here can be ignored
        GameConfSPtr original_config;
        Result();
        Result(bool, GameConfSPtr&&);
    };
    Result stop();
    GameConfiguration* getOriginalConfiguration() const;
    const GameConfSPtr& getOriginalConfigurationSharedPtr() const;

    private:
    using Fighteriterator = std::forward_list<PlayerFighter>::iterator;

    std::forward_list<PlayerFighter> fighters; ///< A list containing all Fighters present in this Game.
    std::unique_ptr<Stage> stage;

    int frame;
    SDL_Rect camera;    ///< The area of the Stage that should be displayed.
    bool running;       ///Indicates if the game is actually running or not.

    GameConfSPtr original_config;     ///< Releases on call to stop().
    //Debug

    //Invariants : 
    // running => stage.get() != nullptr : Si la game est lancée, le pointeur stage doit être valide

};