#ifndef ITAPE_DEVICE_EXCEPTIONS_HPP
#define ITAPE_DEVICE_EXCEPTIONS_HPP

#include <exception>
#include <string>

class TapeDeviceException: public std::exception
{
public:
  TapeDeviceException(std::string error) :
    what_message_(std::move(error))
  {  }

  const char* what() const noexcept override 
  {
    return what_message_.c_str();
  }

private:
  std::string what_message_;
};

class ReadFailedException: public TapeDeviceException
{
  using TapeDeviceException::TapeDeviceException;
};

class WriteFailedException: public TapeDeviceException
{
  using TapeDeviceException::TapeDeviceException;
};

class EndReachedException: public TapeDeviceException
{
  using TapeDeviceException::TapeDeviceException;
};

class BeginReachedException: public TapeDeviceException
{
  using TapeDeviceException::TapeDeviceException;
};

#endif
