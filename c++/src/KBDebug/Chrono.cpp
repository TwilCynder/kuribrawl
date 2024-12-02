#include "KBDebug/Chrono.h"

using namespace std::chrono;

/**
 * @brief Starts the chrono, by setting the start date to the current date.
 * 
 */
void Chrono::start(){
    start_time = system_clock::now();
}

/**
 * @brief Returns the time elapsed since the chrono was started, in nanoseconds.  
 * Does not stop the chrono.
 * @return long long int 
 */
long long int Chrono::end(){
    auto diff = duration_cast<nanoseconds>(std::chrono::high_resolution_clock::now() - start_time).count();
    return (diff);
}

double Chrono::endSec(){
    return end() / 1000000000.0;
}