#pragma once
#include <exception>
#include <string>

/**
 * \brief Exception representing a fatal error that stops Solarus.
 */
class KBFatal : public std::exception {

  public:

    explicit KBFatal(const std::string& error_message);
    virtual const char* what() const noexcept override;

  private:

    const std::string error_message;
};