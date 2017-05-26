#pragma once

#include <memory>
#include <boost/asio.hpp>
#include <boost/circular_buffer.hpp>
#include <mutex>
#include <condition_variable>

namespace jm
{
  namespace filter
  {
    class ClientFilter;
  }

  namespace net
  {
    class ClientConnection
      : public std::enable_shared_from_this<ClientConnection>
    {
      /*
      prevents anybody from creating this class
      */
      struct key {};

    public:
      typedef std::shared_ptr<ClientConnection> ptr;

      static ptr Create(boost::asio::io_service& svc);

      ClientConnection(key, boost::asio::io_service& svc);

      ~ClientConnection();

      auto& Socket() {
        return socket_;
      }

      void Start();

      void Write(const std::string& s);



    private:
      void HandleWrite(const boost::system::error_code&, size_t);
      void HandleRead(const boost::system::error_code&, size_t);
      void StartRead();
      boost::asio::ip::tcp::socket socket_;

      std::deque<std::unique_ptr<char[]>> outbuf_;
      std::vector<char> inbuf_;
      std::mutex mutex_;
      std::condition_variable cond_;
      std::shared_ptr<filter::ClientFilter> filter_;
    };


  }
}
