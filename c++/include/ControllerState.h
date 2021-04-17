#pragma once

#define MAX_BUTTONS_NB 32
#define MAX_STICKS_NB 4

/**
 * @brief 
 * 
 */

struct AxisState {
    int x;
    int y;
    int z;
};

struct ControllerState {
    bool buttons[MAX_BUTTONS_NB];
    AxisState sticks[MAX_STICKS_NB]; 
};