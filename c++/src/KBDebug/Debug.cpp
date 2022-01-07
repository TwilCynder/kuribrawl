#include "KBDebug/Debug.h"

/**
 * @brief Logs a boolean value
 * 
 * @tparam  Specialization : bool
 * @param value 
 */
template<>
void Debug::log(bool b){
    cout << ((b) ? "True" : "False") << '\n' << std::flush;
}