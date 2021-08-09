#include "Debug.h"

template <typename T>
void Debug::log(T value){
    cout << value << '\n' << std::flush;
}

template void Debug::log(char value);
template void Debug::log(char* value);
template void Debug::log(std::string value);
template void Debug::log(const char* value);
template void Debug::log(int value);
template void Debug::log(double value);
template void Debug::log(void* value);
template void Debug::log(long value);
template void Debug::log(long long value);
template void Debug::log(Uint64 value);
template void Debug::log(signed char value);

template<>
void Debug::log(bool b){
    cout << ((b) ? "True" : "False") << '\n' << std::flush;
}