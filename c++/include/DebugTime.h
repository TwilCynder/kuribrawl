
#pragma once
#include <chrono>
#include <ctime>  

class DebugTime {
    std::chrono::high_resolution_clock::time_point start_time;
    public:
    void start();
    long long int end();
};