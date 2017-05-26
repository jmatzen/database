#include "ClientConnection.h"
#include <iostream>


using namespace boost;
using namespace boost::asio;
using boost::asio::ip::tcp;

ClientConnection::ClientConnection(key, io_service& svc)
	: socket_(svc)
{
	inbuf_.resize(1024);
}

void ClientConnection::Start()
{
	auto hello = std::string("hello, world\n");
	Write(hello);
}

void ClientConnection::Write(const std::string& s)
{
	std::unique_lock<std::mutex> lk(mutex_);
	outbuf_.emplace_back(
		std::vector<char>(s.begin(), s.end()));

	async_write(
		socket_,
		boost::asio::buffer(outbuf_.back()),
		[this, p = shared_from_this()](auto e, auto sent) 
	{
		HandleWrite(e, sent);
	});

	async_read(
		socket_,
		boost::asio::buffer(inbuf_),
		[this, p = shared_from_this()](auto e, auto r)
	{
		HandleRead(e, r);
	});
}

void ClientConnection::HandleWrite(
	const system::error_code& ec,
	size_t count)
{
	outbuf_.pop_front();
}

void ClientConnection::HandleRead(
	const system::error_code& ec,
	size_t count)
{
	std::cerr << "read " << ec << " " << count << std::endl;
	if (count > 0) {
		std::cout.write(inbuf_.data(), count);
		Write(std::string(inbuf_.begin(), inbuf_.begin() + count));
	}
}
