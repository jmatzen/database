#include "ClientFilter.h"
#include "ClientConnection.h"
#include "Logging.h"

using namespace jm::net;
using namespace jm::filter;
using namespace jm;


ClientFilter::ClientFilter(ClientConnection::ptr connection)
  : connection_(connection)
{

}

void ClientFilter::Read(const char* buffer, size_t size)
{
  input_.insert(input_.end(), buffer, buffer + size);
  size_t eol;
  for (;;) {
    eol = input_.find('\n');
    if (eol == -1)
      break;
    Parse(std::string(input_.begin(), input_.begin() + eol));
    input_.erase(input_.begin(), input_.begin() + eol + 1);
  } 
}

void ClientFilter::Write(const char* buffer, size_t size)
{
  auto c = connection_.lock();
  if (c) {
    c->Write(std::string(buffer, buffer + size));
  }
}

void ClientFilter::Parse(const std::string& s)
{
  log(LOG_TRACE, "input: %s", s);
}
