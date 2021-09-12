#include "DebugTime.h"

using namespace std::chrono;

void DebugTime::start(){
    start_time = system_clock::now();
}

long long int DebugTime::end(){
    auto diff = duration_cast<nanoseconds>(std::chrono::high_resolution_clock::now() - start_time).count();
    return (diff);
}