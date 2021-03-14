#include "KBException.h"

/**
 * \brief Creates a fatal error that should stop Solarus.
 * \param error_message A message describing the error.
 */
KBFatal::KBFatal(const std::string& error_message):
    error_message(error_message) {

}

/**
 * \brief Returns the error message.
 * \return A message describing the error.
 */
const char* KBFatal::what() const noexcept {
  return error_message.c_str();
}