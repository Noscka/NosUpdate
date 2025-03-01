#pragma once
#include <NosUpdate/WinVersion.hpp>
#include <NosUpdate/Requests.hpp>
#include <NosLib/Logging.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <iostream>

namespace
{
	namespace bSSL = boost::asio::ssl;
	namespace bIP = boost::asio::ip;
}

class TLSConnection
{
	using ASIOContext = boost::asio::io_context;
	using ASIOTLSContext = boost::asio::ssl::context;
	using TCPStream = bIP::tcp::socket;
	using TLSStream = bSSL::stream<bIP::tcp::socket>;
	using NosLog = NosLib::Logging;

public:
	static TLSConnection* CreateConnection(boost::asio::io_context&, bSSL::context&);
	TLSStream& GetTLSSocket();
	TCPStream& GetSocket();
	std::string GetLocalEndpoint();
	std::string GetRemoteEndpoint();

	/* Thread entrance */
	void start();
	NosUpdate::Request::Ptr GetClientsRequest();
	void HandleRequest(NosUpdate::Request::Ptr&);

	virtual ~TLSConnection() = default;
protected:
	TLSStream TLSSocket;

	TLSConnection() = default;
	inline TLSConnection(ASIOContext& io_context, ASIOTLSContext& ssl_context) : TLSSocket(io_context, ssl_context)
	{}
private:
	/* Prevent copying */
	TLSConnection(const TLSConnection&);
	const TLSConnection& operator=(const TLSConnection&);
};