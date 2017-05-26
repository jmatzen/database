#include "Server.h"
#include "ClientConnection.h"

using namespace boost;
using namespace boost::asio;
using boost::asio::ip::tcp;

Server::Server(io_service& iosvc)
	: acceptor_(iosvc, ip::tcp::endpoint(ip::tcp::v4(), 6700))
{
	StartAccept();
}

void Server::StartAccept()
{
	auto con = ClientConnection::Create(acceptor_.get_io_service());
	acceptor_.async_accept(
		con->Socket(),
		[this,con](auto& e) { HandleAccept(con, e); }
	);
}

void Server::HandleAccept(
	std::shared_ptr<ClientConnection> con,
	const boost::system::error_code& e)
{
	if (!e)
		con->Start();
	StartAccept();
}
