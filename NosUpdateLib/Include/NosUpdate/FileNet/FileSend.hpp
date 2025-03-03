#pragma once
#include <NosUpdate/Definitions.hpp>

#include <boost/asio/ssl.hpp>
#include <boost/asio.hpp>

namespace NosUpdate
{
	void SendFile(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>&);
}