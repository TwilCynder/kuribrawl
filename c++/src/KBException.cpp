#include "KBException.h"

/**
 * \brief Creates a fatal error that should stop the game.
 * \param error_message A message describing the error.
 */
KBFatal::KBFatal(const std::string& error_message):
    data(nullptr),
    error_message(error_message)
{
}

/**
 * \brief Returns the error message.
 * \return A message describing the error.
 */
const char* KBFatal::what() const noexcept {
    return error_message.c_str();
}

void KBFatal::setData(void* data_){
    data = data_;
}

void* KBFatal::getData(){
    return data;
}

bool KBFatal::informPlayer(){
    return false;
}

KBFatalExplicit::KBFatalExplicit(const std::string& error_message):KBFatal(error_message){}

bool KBFatalExplicit::informPlayer(){
    return true;
}