#pragma once

#include <memory>
#include <boost/asio.hpp>
#include <boost/circular_buffer.hpp>
#include <mutex>
#include <condition_variable>

class ClientConnection
	: public std::enable_shared_from_this<ClientConnection>
{
	/*
		prevents anybody from creating this class
	 */
	struct key {};

public:
	typedef std::shared_ptr<ClientConnection> ptr;

	static ptr Create(boost::asio::io_service& svc) {
		return std::make_shared<ClientConnection>(key(), svc);
	}

	ClientConnection(key, boost::asio::io_service& svc);

	auto& Socket() {
		return socket_;
	}

	void Start();

	void Write(const std::string& s);



private:
	void HandleWrite(const boost::system::error_code&, size_t);
	void HandleRead(const boost::system::error_code&, size_t);

	boost::asio::ip::tcp::socket socket_;
	
	std::deque<std::vector<char>> outbuf_;
	std::vector<char> inbuf_;
	std::mutex mutex_;
	std::condition_variable cond_;
};

