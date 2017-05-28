#pragma once

#include <memory>

namespace jm
{
  class Service
    : public std::enable_shared_from_this<Service>
  {
  public:
    virtual ~Service() {}
  };
}
