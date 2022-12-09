#include "KBDebug/Debug.h"

Logger Debug::out(std::cout);
Logger Debug::err(std::cerr);

/**
 * @brief Logs a boolean value
 * 
 * @tparam  Specialization : bool
 * @param value 
 */
template<>
void Debug::log(bool b){
    #ifdef DEBUG
    out << ((b) ? "True" : "False") << '\n' << std::flush;
    #endif 
}