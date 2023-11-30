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

    void registerInput(Input input, int element, ElementType element_type, int data);
    void registerInput(Input input, int element, ElementType element_type = ElementType::UNKNOWN, Kuribrawl::Direction direction = Kuribrawl::Direction::NONE, int data = 0);
    void updateInputs();

    //Debug
    int getInputsNumber() const;
    private:
    PlayerFighter* fighter;   /**< The PlayerFighter this InputManager will manage inputs for. Pointer validity : can be invalidated if a fighter is moved. Can't be invalidated otherwise if not modified after construction : if the fighter is deleted, this is too (unique_ptr).*/

    std::deque<RegisteredInput> inputQ; ///< Queue (FIFO) of registered Inputs.
};