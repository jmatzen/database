#include "Database.h"
#include "Logging.h"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>

using jm::Database;
using jm::log;
namespace rj = rapidjson;

void Database::Insert(const rj::Value& value) {
  // convert value to a string
  rj::StringBuffer buf;
  rj::Writer<rj::StringBuffer> writer(buf);
  value.Accept(writer);
  std::unique_lock<std::mutex> lk(mutex_);
  std::streamoff offset = logStream_->seekp(0, std::ios::end).tellp();
  std::string s = buf.GetString();
  //log(jm::LOG_INFO, "offset=%d", offset);
  logStream_->write(s.c_str(), s.length());
}

Database::Database(streamptr&& p)
  : logStream_(std::move(p))
{
}