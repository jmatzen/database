#include "ClientFilter.h"
#include "ClientConnection.h"
#include "Logging.h"
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

using namespace jm::net;
using namespace jm::filter;
using namespace jm;
namespace rj = rapidjson;


ClientFilter::ClientFilter(ClientConnection::ptr connection)
  : connection_(connection)
{

}

void ClientFilter::Read(const char* buffer, size_t size)
{
  input_.insert(input_.end(), buffer, buffer + size);
  for (;;) {
    auto it = std::find(input_.begin(), input_.end(), '\n');
    if (it == input_.end())
      break;
    Parse(std::distance(input_.begin(), it));
    input_.erase(input_.begin(), it+1);
  } 
}

void ClientFilter::Write(const char* buffer, size_t size)
{
  auto c = connection_.lock();
  if (c) {
    c->Write(std::string(buffer, buffer + size));
  }
}

void ClientFilter::Parse(size_t length)
{
  rj::Document d;
  d.Parse(input_.data(), length);
  if (!d.IsObject()) {
    log(LOG_WARN, "failed to parse!");
  }
  else {
    HandleDocument(d);
  }
}

void ClientFilter::HandleDocument(const rj::Document& doc)
{
  rj::StringBuffer buf;
  rj::PrettyWriter<rj::StringBuffer> writer(buf);
  doc.Accept(writer);
  log(LOG_TRACE, "%s", buf.GetString());

}
