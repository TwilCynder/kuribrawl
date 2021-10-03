#pragma once
#include "Fighter.h"
#include "ControllerType.h"
#include "util.h"
#include "inputs.h"

#define CONTROL_STICK_FRAME_BUFFER 1 //TODO make it customizable ?

class Port;
class InputManager;
class Binding;
class PortOptimizationData;
class RegisteredInput;

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
    void updateInputsStates();
    void resolveInputs();
    void handleButtonPress (int button );
    void handleTriggerPress(int trigger);
    void handleStickFlick(Kuribrawl::Direction direction); //currently only for the second stick

    Port* getPort() const;
    void setPort(Port* port);
    void unsetPort();
    Binding* getInputBinding() const;
    void initPortOptimizationData(PortOptimizationData& pod) const;

    int  handleInput(RegisteredInput& input);

    static void initInputHandlers(); 
    using InputHandler = int (PlayerFighter::*)(RegisteredInput&); ///< Function applying the effect of an Input to a Fighter.  

    private:
    InputManager* getInputManager() const;
    //Inputs
    void checkStickState();
    void init_control_stick_buffer();
    void swap_control_stick_buffer();
    void update_control_stick_buffer(const Vector&, const Vector&);
    void updateDirectionControlState();


    Kuribrawl::Direction getDirection4(const Kuribrawl::Vector& stick_state) const;
    Kuribrawl::DirectionIG getDirection4IG(const Kuribrawl::Vector& stick_state) const;

    Kuribrawl::Direction getControlDirection4() const;
    Kuribrawl::DirectionIG getControlDirection4IG() const;

	jumpY decideGroundedJumpYType() const override;
    jumpX decideJumpXType() const override;

    Port* port;         ///<Port controlling this Fighter. Pointer validity : dla merde
    ControllerType::ControllerVals current_controller_vals ; //Set when the port is set
    Binding* input_binding; ///< Pointer validity is a validist concept
    bool valid_port;
    std::unique_ptr<InputManager> input_manager;    ///< InputManager used to process the input made by the Port.
    Vector control_stick_buffer[CONTROL_STICK_FRAME_BUFFER];
    Vector current_direction_control_state;

    static InputHandler input_handlers[Input::TOTAL];   ///< Array associating every Input to an InputHandler function that will apply the effect of this Input to a Fighter.

    int jump_manager(RegisteredInput& input, jumpY type);
    int InputHandler_Jump(RegisteredInput& input);
    int InputHandler_ShortHop(RegisteredInput& input);
    int InputHandler_SmashStickSide(RegisteredInput& input);
    int InputHandler_SmashStickDown(RegisteredInput& input);
    int InputHandler_SmashStickUp(RegisteredInput& input);
    int InputHandler_Attack(RegisteredInput& input);
};