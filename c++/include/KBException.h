#pragma once
#include <string>
#include <exception>

/**
 * \brief Exception representing a fatal error that stops Solarus.
 */
class KBFatal : public std::exception {

  public:

    explicit KBFatal(const std::string& error_message);
    virtual const char* what() const noexcept override;
    void setData(void*);
    void* getData();
    virtual const char* userMessage() const;

  private:
    void* data;

  protected:
    const std::string error_message;
};

class KBFatalExplicit : public KBFatal {
  public:
    explicit KBFatalExplicit(const std::string& error_message);
    const char* userMessage() const;
};

class KBFatalDetailed : public KBFatal {
  public:
    explicit KBFatalDetailed(const std::string& error_message, const std::string& user_message);
    const char* userMessage() const;

  private:
    const std::string user_message;
};