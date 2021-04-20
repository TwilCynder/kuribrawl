#pragma once 
#include "Fighter.h"

class Port;
class InputManager;

/**
 * @brief A Fighter controlled by a player (i.e.\ by a Port).
 * Contains all information needed by a Fighter to manage inputs from a Port.
 */
class PlayerFighter : public Fighter {
    public:

    PlayerFighter(Champion* model_);
    PlayerFighter(Champion* model_, int x, int y);

    //Inputs
    void updateInputs();

    InputManager* getInputManager() const;
    Port* getPort() const;
    void setPort(Port* port);

    private:
    //Inputs
    void checkStickState();

    Port* port;         ///<Port controlling this Fighter. Pointer validity : dla merde
    std::unique_ptr<InputManager> input_manager;    ///< InputManager ised to process the input made by the Port.
};