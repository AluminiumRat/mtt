#pragma once
//-----------------------------------------------------------------------------
// Write log to std::ostream. Default is std::cout
// Use "mtt::Log()" to write messages to log stream. For example:
// mtt::Log() << "Message :" << messageString << " Data:" << dataValue;

#include <iostream>
#include <mutex>
#include <time.h>

namespace mtt
{
  class Log
  {
  public:
    inline Log();
    Log(const Log&) = delete;
    Log& operator = (const Log&) = delete;
    inline ~Log();

    template <typename DataType>
    inline Log& operator << (const DataType& data);

    static void setStream(std::ostream& stream);

  private:
    static std::mutex _writeMutex;
    static std::ostream* _stream;
    std::lock_guard<std::mutex> _mutexLock;
  };

  inline Log::Log() :
    _mutexLock(_writeMutex)
  {
    time_t currentTime = time(nullptr);
    (*_stream) << currentTime << " : ";
  }

  inline Log::~Log()
  {
    (*_stream)<<(std::endl);
    _stream->flush();
  }

  template <typename DataType>
  inline Log& Log::operator << (const DataType& data)
  {
    (*_stream) << (data);
    return *this;
  }
}