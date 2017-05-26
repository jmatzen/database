#pragma once

#include <boost/format.hpp>
#include <iostream>

namespace jm
{
  enum Level
  {
    LOG_TRACE,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR, // should always be fatal
  };

  namespace logging::detail
  {
    void write(Level level, const char* buffer);

    class LogBuilder
    {
    public:
      template<typename... T>
      LogBuilder(Level level, const char format[], const T&... args)
        : level_(level)
        , fmt_(format) 
      {
        append(args...);
      }

      
      ~LogBuilder();

      LogBuilder& append() {
        return *this;
      }

      template<typename T>
      LogBuilder& append(const T& arg) {
        fmt_ % arg;
        return *this;
      }

      template<typename T, typename... Args>
      LogBuilder& append(const T& arg, const Args&... args) {
        fmt_ % arg;
        append(args...);
        return *this;
      }

    private:
      Level level_;
      boost::format fmt_;
    };
  }

  template<typename... Args>
  void log(Level level, const char format[], const Args&... args) {
    logging::detail::LogBuilder b(level, format, args...);
  }
}