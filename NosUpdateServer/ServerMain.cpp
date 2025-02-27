#include <NosUpdate/WinVersion.hpp>

#include <NosUpdate/Helper.hpp>
#include "Header/TLSServer.hpp"
#include "Header/TLSConnection.hpp"

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

	TLSServer updateServer(io_context, ssl_context, 8100);

	updateServer.AcceptLoop();
	
	printf("Press any button to continue"); getchar();
	return 0;
}