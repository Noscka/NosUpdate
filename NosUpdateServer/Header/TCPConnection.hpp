#pragma once
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <NosUpdate/Request.hpp>
#include <iostream>

namespace
{
	namespace bSSL = boost::asio::ssl;
	namespace bIP = boost::asio::ip;
}

class TLSConnection
{
	using TCPStream = bIP::tcp::socket;
	using TLSStream = bSSL::stream<bIP::tcp::socket>;

public:
	static TLSConnection* CreateConnection(boost::asio::io_context&, bSSL::context&);
	TLSStream& GetTLSSocket();
	TCPStream& GetSocket();

	/* Thread entrance */
	void start();
	NosUpdate::Request::Ptr GetClientsRequest();
	void HandleRequest(NosUpdate::Request::Ptr&);

	virtual ~TLSConnection() = default;
protected:
	TLSStream TLSSocket;

	TLSConnection() = default;
	inline TLSConnection(boost::asio::io_context& io_context, bSSL::context& ssl_context) : TLSSocket(io_context, ssl_context)
	{}
private:
	/* Prevent copying */
	TLSConnection(const TLSConnection&);
	const TLSConnection& operator=(const TLSConnection&);
};