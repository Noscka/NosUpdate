#pragma once

#include <boost/asio.hpp>

#include "Definitions.hpp"

namespace NosUpdate
{
	std::string StreamBufferToString(const boost::asio::streambuf& streamBuffer, const size_t& bytesReceived)
	{
		auto bufSeqStart = boost::asio::buffers_begin(streamBuffer.data());
		auto bufSeqEnd = boost::asio::buffers_begin(streamBuffer.data()) + bytesReceived - NosUpdate::GetDelimiter().size();
		return std::string(bufSeqStart, bufSeqEnd);
	}

	/*template<typename SyncWriteStream, typename BufferSequence>
	void SimpleWrite(const SyncWriteStream& socket, BufferSequence streamBuf)
	{
		boost::asio::write(socket, streamBuf);
		boost::asio::write(socket, boost::asio::buffer(NosUpdate::GetDelimiter()));
	}*/
}