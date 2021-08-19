#pragma once
#include "Fighter.h"
#include "ControllerType.h"
#include "util.h"

#define CONTROL_STICK_FRAME_BUFFER 1 //TODO make it customizable ?

class Port;
class InputManager;
class Binding;
class PortOptimizationData;

using namespace Kuribrawl;

/**
 * @brief A Fighter controlled by a player (i.e.\ by a Port).
 * Contains all information needed by a Fighter to manage inputs from a Port.
 */
class PlayerFighter : public Fighter {
    public:

    PlayerFighter(Game&, Champion* model_);
    PlayerFighter(Game&, Champion* model_, int x, int y);
    PlayerFighter(Game&, Champion* model_, int x, int y, Port& port);
    ~PlayerFighter();

    //Inputs
    void updateInputs();
    void updateSticks();
    void handleButtonPress (int button );
    void handleTriggerPress(int trigger);
    
    Port* getPort() const;
    void setPort(Port* port);
    void unsetPort();
    Binding* getInputBinding() const;
    void initPortOptimizationData(PortOptimizationData& pod) const;

    private:
    InputManager* getInputManager() const;
    //Inputs
    void checkStickState();
    void init_control_stick_buffer();
    void swap_control_stick_buffer();
    void update_control_stick_buffer(const Vector&, const Vector&, const ControllerType::ControllerVals&);
    void updateDirectionControlState(ControllerType::ControllerVals controller_vals);

	jumpY decideGroundedJumpYType() const override;

    Port* port;         ///<Port controlling this Fighter. Pointer validity : dla merde
    Binding* input_binding; ///< Pointer validity is a validist concept
    bool valid_port;
    std::unique_ptr<InputManager> input_manager;    ///< InputManager used to process the input made by the Port.
    Vector control_stick_buffer[CONTROL_STICK_FRAME_BUFFER];
    Vector current_direction_control_state;
};