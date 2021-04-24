#pragma once
#include "Fighter.h"
#include "ControllerType.h"
#include "util.h"

#define CONTROL_STICK_FRAME_BUFFER 1 //TODO make it customizable ?

class Port;
class InputManager;

using namespace Kuribrawl;

/**
 * @brief A Fighter controlled by a player (i.e.\ by a Port).
 * Contains all information needed by a Fighter to manage inputs from a Port.
 */
class PlayerFighter : public Fighter {
    public:

    PlayerFighter(Champion* model_);
    PlayerFighter(Champion* model_, int x, int y);
    ~PlayerFighter();

    //Inputs
    void updateInputs();
    void updateSticks();

    InputManager* getInputManager() const;
    Port* getPort() const;
    void setPort(Port* port);

    private:
    //Inputs
    void checkStickState();
    void init_control_stick_buffer();
    void swap_control_stick_buffer();
    void update_control_stick_buffer(const Vector&, const Vector&, const ControllerType::ControllerVals&);

	jumpY decideGroundedJumpYType() const override;

    Port* port;         ///<Port controlling this Fighter. Pointer validity : dla merde
    bool valid_port;
    std::unique_ptr<InputManager> input_manager;    ///< InputManager ised to process the input made by the Port.
    Vector control_stick_buffer[CONTROL_STICK_FRAME_BUFFER];
};