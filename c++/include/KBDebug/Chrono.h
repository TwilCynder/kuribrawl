
#pragma once
#include <chrono>
#include <ctime>  

/**
 * @brief A chrono, used to measure time.
 * 
 */
class Chrono {
    std::chrono::high_resolution_clock::time_point start_time; /**<Point in time (date) when the chrono was started*/
    public:
    void start();
    long long int end();   
    double endSec(); 
};