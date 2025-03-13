#pragma once
#include <NosUpdate/Definitions.hpp>

#include <boost/filesystem.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio.hpp>

#include <fstream>

namespace NosUpdate
{
	template<typename SyncWriteStream>
	inline void ReceiveFile(SyncWriteStream& socket, const std::string& filename, uint64_t expectedContentsize, const uint64_t& resumePos = 0)
	{
		namespace bfilesys = boost::filesystem;

		bfilesys::path filePath(filename);
		bfilesys::path parentDir(filePath.parent_path());
		bfilesys::create_directories(parentDir);

		std::ofstream oFStream(filename, std::ios::binary | std::ios::trunc);

		expectedContentsize -= resumePos;
		uint64_t TotalDataReceived = 0, TotalFileSize = expectedContentsize;

		while (expectedContentsize != 0)
		{
			uint64_t receiveAmount = Definitions::FileSegmentSize;

			if (expectedContentsize < Definitions::FileSegmentSize)
			{
				receiveAmount = expectedContentsize;
			}

			std::vector<Definitions::Byte> receivedData(receiveAmount);

			size_t ReceivedByteCount = boost::asio::read(socket, boost::asio::buffer(receivedData));

			expectedContentsize -= ReceivedByteCount;

			/* Convert to string temporarily to allow for writing into file */
			std::string TempString((char*)receivedData.data(), ReceivedByteCount);

			/* write content into file */
			oFStream.write(TempString.c_str(), TempString.size());
		}

		oFStream.close();
	}
}