#ifdef WIN32
#include <SDKDDKVer.h>
#endif // WIN32

#include <boost/thread.hpp>

#include "Header/Helper.hpp"
#include "Header/TCPConnection.hpp"

#include <iostream>

int main()
{
	boost::asio::io_context io_context;
	boost::asio::ip::tcp::acceptor acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8100));

	printf("Server started\n");
	printf("Listening on: %s\n", EndpointAsString(acceptor.local_endpoint()).c_str());

	while (true)
	{
		/* tcp_connection object which allows for managed of multiple users */
		tcp_connection* newConSim = tcp_connection::create(io_context);

		boost::system::error_code error;

		/* accept incoming connection and assigned it to the tcp_connection object socket */
		acceptor.accept(newConSim->ConSocket(), error);

		/* if no errors, create thread for the new connection */
		if (!error)
		{
			boost::thread* ClientThread = new boost::thread(boost::bind(&tcp_connection::start, newConSim));
		}
	}

	printf("Press any button to continue"); getchar();
	return 0;
}