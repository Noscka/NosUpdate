#include <NosUpdate/FileNet/FileReceive.hpp>

#include <boost/array.hpp>

#include <fstream>

void NosUpdate::ReceiveFile(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& socket, uint64_t expectedContentsize)
{
	std::string filename = "TestData.txt";
	uint64_t resumePos = 0;

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

	return;
}