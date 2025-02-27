#include <NosUpdate/WinVersion.hpp>

#include <boost/thread.hpp>
#include <boost/asio/ssl.hpp>

#include <NosUpdate/Helper.hpp>
#include "Header/TCPConnection.hpp"

#include <iostream>

int main()
{
	boost::asio::io_context io_context;

	boost::asio::ssl::context ssl_context(boost::asio::ssl::context::sslv23);
	ssl_context.set_options(
		boost::asio::ssl::context::default_workarounds |
		boost::asio::ssl::context::no_sslv2 |
		boost::asio::ssl::context::single_dh_use);
	ssl_context.use_certificate_chain_file("server.pem");
	ssl_context.use_private_key_file("server.pem", boost::asio::ssl::context::pem);
	ssl_context.use_tmp_dh_file("dh2048.pem");

	boost::asio::ip::tcp::acceptor acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8100));

	printf("Server started\n");
	printf("Listening on: %s\n", NosUpdate::EndpointAsString(acceptor.local_endpoint()).c_str());

	while (true)
	{
		/* TLSConnection object which allows for managed of multiple users */
		TLSConnection* newTlsCon = TLSConnection::CreateConnection(io_context, ssl_context);

		boost::system::error_code error;

		/* accept incoming connection and assigned it to the TLSConnection object socket */
		acceptor.accept(newTlsCon->GetSocket(), error);

		/* if no errors, create thread for the new connection */
		if (error)
		{
			delete newTlsCon;
			continue;
		}
		boost::thread* ClientThread = new boost::thread(boost::bind(&TLSConnection::start, newTlsCon));
	}

	printf("Press any button to continue"); getchar();
	return 0;
}