#pragma once
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/array.hpp>

#include "Helper.hpp"

#include <iostream>

class tcp_connection
{
private:
	boost::asio::ip::tcp::socket socket;

	tcp_connection(boost::asio::io_context& io_context) : socket(io_context) {}
public:
	static tcp_connection* create(boost::asio::io_context& io_context)
	{
		return new tcp_connection(io_context);
	}

	boost::asio::ip::tcp::socket& ConSocket() { return socket; }

	void start()
	{
		printf("Client Connected from %s\n", EndpointAsString(socket.local_endpoint()).c_str());

		try
		{
			boost::system::error_code error;
			while (!error)
			{
				boost::array<char, 128> buf;
				boost::system::error_code error;

				size_t len = socket.read_some(boost::asio::buffer(buf), error);

				if (error == boost::asio::error::eof)
					break; // Connection closed cleanly by client.
				else if (error)
					throw boost::system::system_error(error); // Some other error.

				std::cout.write(buf.data(), len);
			}
		}
		catch (std::exception& e)
		{
			fprintf(stderr, "%s\n", e.what());
		}
	}
};