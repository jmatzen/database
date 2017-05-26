#include "Logging.h"
#include <chrono>
#include <thread>
#include <mutex>
#include <sstream>

namespace
{
  std::mutex s_mutex;
}

jm::logging::detail::LogBuilder::~LogBuilder()
{
  std::unique_lock<std::mutex> lk(s_mutex);

  // format the current timestamp
  auto now = std::chrono::system_clock::now();
  auto now_time = std::chrono::system_clock::to_time_t(now);
  char currentTimeAsAString[256];
  std::strftime(currentTimeAsAString
    , sizeof(currentTimeAsAString)
    , "%y-%m-%d %H:%M:%S"
    , localtime(&now_time));

  // print the current time
  std::cout << currentTimeAsAString << " ";

  // print the log level
  switch (level_) {
  case LOG_TRACE: std::cout << "TRC "; break;
  case LOG_DEBUG: std::cout << "DBG "; break;
  case LOG_INFO: std::cout << "INF "; break;
  case LOG_WARN: std::cout << "WRN "; break;
  case LOG_ERROR: std::cout << "ERR "; break;
  }

  // print the thread ID
  std::cout << std::this_thread::get_id() << " ";

  // print the log text and a new line
  std::cout << fmt_ << std::endl;

  // abort on error
  if (level_ == LOG_ERROR)
    std::abort();
}

