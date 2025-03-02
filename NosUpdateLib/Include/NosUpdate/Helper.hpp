#pragma once

#include "Definitions.hpp"
#include <boost/asio.hpp>

namespace NosUpdate
{
	inline std::string EndpointAsString(boost::asio::ip::tcp::endpoint Endpoint)
	{
		return std::format("{}:{}", Endpoint.address().to_v4().to_string(), std::to_string(Endpoint.port()));
	}

	inline std::string StreamBufferToString(const boost::asio::streambuf& streamBuffer, const size_t& bytesReceived)
	{
		auto bufSeqStart = boost::asio::buffers_begin(streamBuffer.data());
		auto bufSeqEnd = boost::asio::buffers_begin(streamBuffer.data()) + bytesReceived - NosUpdate::GetDelimiter().size();
		return std::string(bufSeqStart, bufSeqEnd);
	}

	template<typename SyncWriteStream, typename BufferSequence>
	inline void SimpleWrite(SyncWriteStream& socket, BufferSequence& streamBuf)
	{
		boost::asio::write(socket, streamBuf);
		boost::asio::write(socket, boost::asio::buffer(NosUpdate::GetDelimiter()));
	}

	template<typename SyncWriteStream, typename BufferSequence>
	inline void SimpleWrite(SyncWriteStream& socket, BufferSequence&& streamBuf)
	{
		boost::asio::write(socket, streamBuf);
		boost::asio::write(socket, boost::asio::buffer(NosUpdate::GetDelimiter()));
	}

	template<typename SyncWriteStream, typename BufferSequence>
	inline void SimpleRead(SyncWriteStream& socket, BufferSequence& streamBuf)
	{
		boost::asio::read_until(socket, streamBuf, NosUpdate::GetDelimiter());
	}
}