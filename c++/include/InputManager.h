#pragma once
#include <deque>
#include "input.h"
#include "inputs.h"

class Port;
class Fighter;

class InputManager {
    public:
    InputManager(Fighter*);

    void registerInput(Input input, Port* port, int element, ElementType element_type, int data);
    void updateInputs();

    static void initInputHandlers(); 

    using InputHandler = int(*)(Fighter* fighter, Port*, RegisteredInput&);   

    //Debug
    int getQSize() const;
    private:
    Fighter* fighter;   //Pointer validity : can be invalidated if a fighter is moved. Can't be invalidated otherwise if not modified after construction : if the fighter is deleted, this is too (unique_ptr).

    static InputHandler input_handlers[Input::TOTAL];

    std::deque<RegisteredInput> inputQ;
};