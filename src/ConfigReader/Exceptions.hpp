#ifndef CONFIG_EXCEPTIONS_HPP
#define CONFIG_EXCEPTIONS_HPP

#include <exception>
#include <string>

class BadConfigException: public std::exception
{
public:
  BadConfigException(std::string error) :
    what_message_(std::move(error))
  {  }

  const char* what() const noexcept override 
  {
    return what_message_.c_str();
  }

private:
  std::string what_message_;
};

class FieldNotFoundException: public BadConfigException
{
  using BadConfigException::BadConfigException;
};

class BadFieldException: public BadConfigException
{
  using BadConfigException::BadConfigException;
};

class FileNotFoundException: public BadConfigException
{
  using BadConfigException::BadConfigException;
};

#endif
