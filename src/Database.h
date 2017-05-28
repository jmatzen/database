#pragma once

#include "Service.h"
#include <rapidjson/fwd.h>
#include <iostream>
#include <mutex>

namespace jm
{
  class Database : public Service
  {
  public:
    using streamptr = std::unique_ptr<std::iostream>;

    Database(streamptr&& p);

    void Insert(const rapidjson::Value& value);

  private:
    std::unique_ptr<std::iostream> logStream_;
    std::mutex mutex_;
  };
}

