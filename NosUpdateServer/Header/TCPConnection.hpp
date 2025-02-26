#pragma once
#include <boost/asio.hpp>

#include <NosUpdate/Request.hpp>

#include "Helper.hpp"

#include <iostream>

class tcp_connection
{
private:
	boost::asio::ip::tcp::socket socket;

	tcp_connection(boost::asio::io_context& io_context) : socket(io_context) {}
public:
	static tcp_connection* create(boost::asio::io_context& io_context);
	boost::asio::ip::tcp::socket& ConSocket();

	/* Thread entrance */
	void start();
	NosUpdate::Request GetClientsRequest();
	void HandleRequest(NosUpdate::Request&);
};