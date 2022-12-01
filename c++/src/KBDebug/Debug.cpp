#include "KBDebug/Debug.h"

Logger out(std::cout);
Logger err(std::cerr);

/**
 * @brief Logs a boolean value
 * 
 * @tparam  Specialization : bool
 * @param value 
 */
template<>
void Debug::log(bool b){
    std::cout << ((b) ? "True" : "False") << '\n' << std::flush;
}