#pragma once
#include <string>
#include <exception>
#include "Util/stringOperations.h"

/**
 * \brief Exception representing a fatal error that stops Kuribrawl.
 */
class KBFatal : public std::exception {

  public:

    explicit KBFatal(const std::string_view& error_message);
    explicit KBFatal(const char* error_message);
    explicit KBFatal(const std::string&& error_message);
    virtual const char* what() const noexcept override;
    void setData(void*);
    void* getData() const;
    virtual const char* userMessage() const;

    template<typename T, typename... Args>
    explicit KBFatal(const char* format, T arg1, Args... args) : 
      KBFatal(Kuribrawl::formatString(format, arg1, args...))
    {}

  private:
    void* data;

  protected:
    const std::string error_message;
};

class KBFatalExplicit : public KBFatal {
  public:
    explicit KBFatalExplicit(const std::string_view& error_message);
    explicit KBFatalExplicit(const std::string&& error_message);
    explicit KBFatalExplicit(const char* error_message);
    const char* userMessage() const;

    template<typename T, typename... Args>
    explicit KBFatalExplicit(const char* format, T arg1, Args... args) : 
      KBFatalExplicit(Kuribrawl::formatString(format, arg1, args...))
    {}
};

class KBFatalDetailed : public KBFatal {
  public:
    explicit KBFatalDetailed(const std::string_view& error_message, const std::string_view& user_message);
    explicit KBFatalDetailed(const std::string&& error_message, const std::string&& user_message);
    explicit KBFatalDetailed(const std::string&& error_message, const std::string_view& user_message);
    explicit KBFatalDetailed(const char* error_message, const char* user_message);
    explicit KBFatalDetailed(const std::string&& error_message, const char* user_message);
    template<typename T, typename... Args>
    explicit KBFatalDetailed(const std::string_view& user_message, const char* error_format, T error_arg1, Args... error_args):
      KBFatalDetailed(user_message, std::move(Kuribrawl::formatString(error_format, error_arg1, error_args...))) {}

    const char* userMessage() const;

  private:
    const std::string user_message;

};
