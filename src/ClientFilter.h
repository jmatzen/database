#pragma once

#include <memory>
#include <string>


namespace jm::net
{
  class ClientConnection;
}

namespace jm::filter
{

  class ClientFilter
    : std::enable_shared_from_this<ClientFilter>
  {
  public:
    ClientFilter(std::shared_ptr<net::ClientConnection> connection);
    void Read(const char* buffer, size_t size);
    void Write(const char* buffer, size_t size);
  private:
    void Parse(const std::string& s);

    std::weak_ptr<net::ClientConnection> connection_;
    std::string input_;
  };
}
