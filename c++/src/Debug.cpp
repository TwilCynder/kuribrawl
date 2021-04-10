#include "Debug.h"

void Debug::log(std::string s){
    cout << s << '\n';
    fflush(stdout);
}

void Debug::log(const char* s){
    cout << s << '\n' << std::flush;
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

void Debug::log(long i){
    cout << i << '\n';
    fflush(stdout);
}

void Debug::log(bool b){
    if (b)
        cout << "True\n" << std::flush;
    else 
        cout << "False\n" << std::flush;
    fflush(stdout);
}