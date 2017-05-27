#include "ClientConnection.h"
#include "ClientFilter.h"
#include "Logging.h"
#include <iostream>


using namespace boost;
using namespace boost::asio;
using namespace jm::net;
using namespace jm::filter;
using boost::asio::ip::tcp;

ClientConnection::ClientConnection(key, io_service& svc)
	: socket_(svc)
{
  jm::log(jm::LOG_INFO, "created client connection %1%", this);
  inbuf_.resize(1);
}

ClientConnection::~ClientConnection()
{
  jm::log(jm::LOG_INFO, "destroyed client connection %1%" , this);
}

void ClientConnection::Start()
{
  auto remote = socket_.remote_endpoint();
  jm::log(jm::LOG_INFO, "remote address is %s:%d", remote.address(), remote.port());
  auto hello = std::string("hello, world\n");
	Write(hello);
  StartRead();
}

void ClientConnection::Write(const std::string& s)
{
	
  // allocate buffer for output
  auto buf = std::make_unique<char[]>(s.length());

  // copy data
  memcpy(buf.get(), s.data(), s.length());

  {
    std::unique_lock<std::mutex> lk(mutex_);
    // store buffer
    outbuf_.push_back(std::move(buf));
  }

  // write to socket
	async_write(
		socket_,
		boost::asio::buffer(outbuf_.back().get(), s.length()),
		[this, p = shared_from_this()](auto e, auto sent) 
	{
		HandleWrite(e, sent);
	});

}

void ClientConnection::HandleWrite(
	const system::error_code& ec,
	size_t count)
{
  std::unique_lock<std::mutex> lk(mutex_);
  outbuf_.pop_front();
}

void ClientConnection::StartRead()
{
  async_read(
    socket_,
    boost::asio::buffer(inbuf_),
    [this, p = shared_from_this()](auto e, auto r)
  {
    HandleRead(e, r);
  });
}

void ClientConnection::HandleRead(
	const system::error_code& ec,
	size_t count)
{
	if (count > 0) {
    filter_->Read(inbuf_.data(), count);
    StartRead();
  }
  else {
    filter_->Read(nullptr, 0); // flush filter
    jm::log(jm::LOG_INFO, "socket is closed count=%d", count);
  }
}


ClientConnection::ptr ClientConnection::Create(boost::asio::io_service& svc) {
  auto con = std::make_shared<ClientConnection>(key(), svc);
  con->filter_ = std::make_shared<ClientFilter>(con);
  return con;
}