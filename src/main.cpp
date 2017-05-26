#include "Server.h"
#include <iostream>
#include <boost/thread.hpp>

using namespace boost;
using namespace boost::asio;
using boost::asio::ip::tcp;

int main() {
	thread_group threadPool;

	try {
		io_service iosrv;
		jm::net::Server server(iosrv);
		for (int i = 0; i < 4; ++i) {
			threadPool.create_thread([&iosrv] 
			{
				iosrv.run();
			});
		}
		threadPool.join_all();
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
