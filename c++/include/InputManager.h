#pragma once
#include <deque>
#include "input.h"
#include "inputs.h"

class Port;
class PlayerFighter;

/**
 * @brief Class keeping track of registered inputs and interpreting them.
 * Doesn't manage *all* inputs, only the ones related to a certain Fighter.
 */
class InputManager {
    public:
    InputManager(PlayerFighter*);

    void registerInput(Input input, Port* port, int element, ElementType element_type, int data = 0);
    void updateInputs();

    static void initInputHandlers(); 

    using InputHandler = int(*)(PlayerFighter* fighter, Port*, RegisteredInput&); ///< Function applying the effect of an Input to a Fighter.  

    //Debug
    int getInputsNumber() const;
    private:
    PlayerFighter* fighter;   /**< The PlayerFighter this InputManager will manage inputs for. Pointer validity : can be invalidated if a fighter is moved. Can't be invalidated otherwise if not modified after construction : if the fighter is deleted, this is too (unique_ptr).*/

    static InputHandler input_handlers[Input::TOTAL];   ///< Array associating every Input to an InputHandler function that will apply the effect of this Input to a Fighter.

    std::deque<RegisteredInput> inputQ; ///< Queue (FIFO) of registered Inputs.
};