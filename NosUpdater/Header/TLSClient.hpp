#pragma once
#include <NosUpdate/WinVersion.hpp>
#include <NosUpdate/Requests.hpp>
#include <NosUpdate/Version.hpp>
#include <NosUpdate/Requests.hpp>
#include <NosUpdate/Responses.hpp>
#include <NosLib/Logging.hpp>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

namespace
{
	namespace bSSL = boost::asio::ssl;
	namespace bIP = boost::asio::ip;
}

class TLSClient
{
	using ASIOContext = boost::asio::io_context;
	using ASIOTLSContext = boost::asio::ssl::context;
	using TCPStream = bIP::tcp::socket;
	using TLSStream = bSSL::stream<bIP::tcp::socket>;
	using NosLog = NosLib::Logging;

public:
	TLSClient(ASIOContext& ioContext, ASIOTLSContext& tlsContext, const std::string& hostname, const uint16_t& port) :
		IOContext(ioContext),
		TLSContext(tlsContext),
		TLSSocket(ioContext, tlsContext),
		Hostname(hostname),
		Port(port)
	{}

	TLSStream& GetTLSSocket();
	TCPStream& GetSocket();
	std::string GetLocalEndpoint();
	std::string GetRemoteEndpoint();

	void Connect(); /* Connects to server */
	void UpdateProgram();

	virtual ~TLSClient() = default;
protected:
	NosUpdate::Version GetNewestVersion();
	NosUpdate::UpdateResponse::Ptr RequestUpdate(const NosUpdate::Version&);
	void ReceivedUpdateFiles(const NosUpdate::UpdateResponse::Ptr&);

	TLSClient() = default;
private:
	ASIOContext& IOContext;
	ASIOTLSContext& TLSContext;
	TLSStream TLSSocket;
	std::string Hostname;
	uint16_t Port;

	/* Prevent copying */
	TLSClient(const TLSClient&);
	const TLSClient& operator=(const TLSClient&);
};