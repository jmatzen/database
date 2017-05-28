#include "Server.h"
#include "AppContext.h"
#include "Database.h"
#include "Logging.h"
#include <iostream>
#include <fstream>
#include <boost/thread.hpp>

using namespace boost;
using namespace boost::asio;
using jm::AppContext;
using jm::Database;
using boost::asio::ip::tcp;

namespace
{
  AppContext appContext;
}


/*
implement app context getter function
*/

AppContext& AppContext::GetAppContext() {
  return appContext;
}

int main() {
	thread_group threadPool;

    auto stream = std::make_unique<std::fstream>(
      "data.dat"
      , std::ios::binary | std::ios::out);

    if (!stream->is_open())
    {
      jm::log(jm::LOG_ERROR, "failed to open database file.");
    }

    appContext.RegisterService<Database>(std::move(stream));

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
