#include "Debug.h"

void Debug::log(std::string s){
    cout << s << '\n';
    fflush(stdout);
}

void Debug::log(int i){
    cout << i << '\n';
    fflush(stdout);
}

void Debug::log(double d){
    cout << d << '\n';
    fflush(stdout);
}

void Debug::log(void * p){
    cout << p << '\n';
    fflush(stdout);
}