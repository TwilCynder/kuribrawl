#include "KBDebug/Debug.h"

template<>
void Debug::log(bool b){
    cout << ((b) ? "True" : "False") << '\n' << std::flush;
}