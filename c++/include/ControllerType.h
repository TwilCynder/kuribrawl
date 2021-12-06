#pragma once
#include <memory>
#include "Binding.h"
#include "ControllerLayout.h"
#include "ControllerVals.h"
#include "Util/util.h"
#include <SDL2/SDL_types.h>

#define DEFAULT_NB_BUTTONS 24
#define DEFAULT_NB_STICKS 3

/**
 * @brief Type of controller.
 * Contains all information needed to manipulate a certain type of controller
 */

class ControllerType {
    public:

    ControllerType();

    const ControllerVals& getControllerVals() const;
    void setControllerVals(int ast, int att, int asst);
  
    ControllerVals default_vals; ///< Maybe useful idk i haven't played the game

    ControllerLayout* getElementLayout() const;
    ControllerLayout& addElementLayout();

    Binding& getDefaultBinding() const;
    Binding* getDefaultBinding();

    private:
    std::unique_ptr<ControllerLayout> element_mapping; //Will not be present for all controllers; only when there is a need to override the SDL one
    std::unique_ptr<Binding> default_binding; ///< The Binding that will be used by default for controllers that are associated with this ControllerType

    int nb_buttons; 
    int nb_sticks; //Sticks include triggers //no they don't //fuck you
};