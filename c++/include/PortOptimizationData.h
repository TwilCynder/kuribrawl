#pragma once

struct ControllerVals;

/**
 * @brief Data cached by a port when a fighter is associated to it, allowing it to avoid doing useless readings
 * 
 */
struct PortOptimizationData {
    bool is_left_trigger_binding;
    bool is_right_trigger_binding;

    PortOptimizationData();
};