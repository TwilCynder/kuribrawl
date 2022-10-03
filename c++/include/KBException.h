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
    virtual const char* what() const noexcept override;
    void setData(void*);
    void* getData();
    virtual const char* userMessage() const;

    template<typename... Args>
    explicit KBFatal(const std::string_view& format, Args... args) : 
      KBFatal(Kuribrawl::formatString(format, args...))
    {}

  private:
    void* data;

  protected:
    const std::string_view error_message;
};

class KBFatalExplicit : public KBFatal {
  public:
    explicit KBFatalExplicit(const std::string_view& error_message);
    const char* userMessage() const;
};

class KBFatalDetailed : public KBFatal {
  public:
    explicit KBFatalDetailed(const std::string_view& error_message, const std::string_view& user_message);
    const char* userMessage() const;

  private:
    const std::string_view user_message;
};