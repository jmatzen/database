#pragma once

#include <boost/asio.hpp>

namespace jm::net
{
  class ClientConnection;

  class Server
  {
  public:
    Server(boost::asio::io_service& iosvc);
  private:
    void StartAccept();

    void HandleAccept(
      std::shared_ptr<ClientConnection> con,
      const boost::system::error_code&);

  private:
    boost::asio::ip::tcp::acceptor acceptor_;
  };
}
