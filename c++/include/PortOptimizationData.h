#pragma once

struct ControllerVals;

/**
 * @brief Data cached by a port when a fighter is associated to it, allowing it to avoid doing useless readings
 * 
 */
struct PortOptimizationData {
    bool read_left_trigger;
    bool read_right_trigger;
    bool read_dpad;

    PortOptimizationData();
};