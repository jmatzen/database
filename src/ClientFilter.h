#pragma once

#include <memory>
#include <vector>
#include <rapidjson/document.h>


namespace jm::net
{
  class ClientConnection;
}

namespace jm::filter
{

  class ClientFilter
    : public std::enable_shared_from_this<ClientFilter>
  {
  public:
    ClientFilter(std::shared_ptr<net::ClientConnection> connection);
    void Read(const char* buffer, size_t size);
    void Write(const char* buffer, size_t size);
  private:
    void Parse(size_t length);
    void HandleDocument(const rapidjson::Document& doc);
    void HandleInsert(const rapidjson::Value& value);

  private:
    std::weak_ptr<net::ClientConnection> connection_;
    std::vector<char> input_;
  };
}
