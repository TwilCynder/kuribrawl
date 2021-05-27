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
    void setData(void*);
    void* getData();
    virtual bool informPlayer();

  private:
    void* data;
    const std::string error_message;
};

class KBFatalExplicit : public KBFatal {
  public:
    explicit KBFatalExplicit(const std::string& error_message);
    bool informPlayer();
};