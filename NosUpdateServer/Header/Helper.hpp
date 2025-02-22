#pragma once
#include <boost/asio.hpp>

std::string EndpointAsString(boost::asio::ip::tcp::endpoint Endpoint)
{
	return std::format("{}:{}", Endpoint.address().to_v4().to_string(), std::to_string(Endpoint.port()));
}