#include "KBException.h"

/**
 * \brief Creates a fatal error that should stop the game.
 * \param error_message A message describing the error.
 */
KBFatal::KBFatal(const std::string_view& error_message_):
    data(nullptr),
    error_message(error_message_)
{
}

/**
 * \brief Creates a fatal error that should stop the game.
 * \param error_message A message describing the error.
 */
KBFatal::KBFatal(const std::string&& error_message_):
    data(nullptr),
    error_message(std::move(error_message_))
{
}

KBFatal::KBFatal(const char* str):
    KBFatal(std::string_view(str))
{}

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

void* KBFatal::getData() const {
    return data;
}

const char* KBFatal::userMessage() const{
    return "Internal error. \nPlease contact the stupid ass developer who made this.";
}

KBFatalExplicit::KBFatalExplicit(const std::string_view& error_message):
    KBFatal(error_message)
{}

KBFatalExplicit::KBFatalExplicit(const std::string&& error_message):
    KBFatal(error_message)
{}

KBFatalExplicit::KBFatalExplicit(const char* error_message):
    KBFatal(error_message)
{}

const char* KBFatalExplicit::userMessage()const{
    return error_message.c_str();
}

KBFatalDetailed::KBFatalDetailed(const std::string_view& error_message, const std::string_view& user_message_):
    KBFatal(error_message), user_message(user_message_)
{}

KBFatalDetailed::KBFatalDetailed(const std::string&& error_message, const std::string_view& user_message_):
    KBFatal(error_message), user_message(user_message_)
{}

KBFatalDetailed::KBFatalDetailed(const std::string&& error_message, const std::string&& user_message_):
    KBFatal(error_message), user_message(std::move(user_message_))
{}

KBFatalDetailed::KBFatalDetailed(const char* error_message, const char* user_message_):
    KBFatal(error_message), user_message(user_message_)
{}

KBFatalDetailed::KBFatalDetailed(const std::string&& error_message, const char* user_message_):
    KBFatal(error_message), user_message(user_message_)
{}

const char* KBFatalDetailed::userMessage()const{
    return user_message.c_str();
}