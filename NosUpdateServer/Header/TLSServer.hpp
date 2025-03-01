#pragma once
#include <NosUpdate/WinVersion.hpp>
#include <NosLib/Logging.hpp>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/thread.hpp>

namespace
{
	namespace bSSL = boost::asio::ssl;
	namespace bIP = boost::asio::ip;
}

class TLSServer
{
	using ASIOContext = boost::asio::io_context;
	using ASIOTLSContext = boost::asio::ssl::context;
	using NosLog = NosLib::Logging;

public:
	TLSServer(ASIOContext& ioContext, ASIOTLSContext& tlsContext, const uint16_t& port) :
		IOContext(ioContext),
		TLSContext(tlsContext),
		ConnectionAcceptor(ioContext, bIP::tcp::endpoint(bIP::tcp::v4(), port))
	{}

	void AcceptLoop();
	void AcceptNewConnection();

	virtual ~TLSServer() = default;
protected:

	TLSServer() = default;
private:
	ASIOContext& IOContext;
	ASIOTLSContext& TLSContext;
	bIP::tcp::acceptor ConnectionAcceptor;

	/* Prevent copying */
	TLSServer(const TLSServer&);
	const TLSServer& operator=(const TLSServer&);
};