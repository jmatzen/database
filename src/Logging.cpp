#include "Logging.h"
#include <chrono>
#include <thread>
#include <mutex>

namespace
{
  std::mutex s_mutex;
}

jm::logging::detail::LogBuilder::~LogBuilder()
{
  std::unique_lock<std::mutex> lk(s_mutex);

  auto now = std::chrono::system_clock::now();
  auto now_time = std::chrono::system_clock::to_time_t(now);
  char buf[256];
  std::strftime(buf, sizeof(buf), "%y-%m-%d %H:%M:%S", localtime(&now_time));
  std::cout << buf << " ";
  switch (level_) {
  case LOG_TRACE: std::cout << "TRC "; break;
  case LOG_DEBUG: std::cout << "DBG "; break;
  case LOG_INFO: std::cout << "INF "; break;
  case LOG_WARN: std::cout << "WRN "; break;
  case LOG_ERROR: std::cout << "ERR "; break;
  }
  std::cout << std::this_thread::get_id() << " ";
  std::cout << fmt_ << std::endl;
  if (level_ == LOG_ERROR)
    std::abort();
}

