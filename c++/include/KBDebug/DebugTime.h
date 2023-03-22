
#pragma once
#include <chrono>
#include <ctime>  

/**
 * @brief A chrono, used to measure time.
 * 
 */
class DebugTime {
    std::chrono::high_resolution_clock::time_point start_time; /**<Point in time (date) when the chrono was started*/
    public:
    ///@brief Starts a time measurement : the start time for subsequent operations is set to right now.
    void start();
    ///@brief Returns, in nanoseconds, the difference between the current time and the start time (when start was called)
    long long int end();   
    ///@brief Returns, in seconds, the difference between the current time and the start time (when start was called)
    double endSec(); 
};