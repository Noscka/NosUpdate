#include <NosUpdate/FileNet/FileReceive.hpp>

#include <boost/array.hpp>

#include <fstream>

void NosUpdate::ReceiveFile(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& socket, uint64_t expectedContentsize)
{
	std::string filename = "NosUpdateServer.exe";
	uint64_t resumePos = 0;

	std::ofstream OutFileStream;

	/* If program is resuming download, append instead of overwriting */
	if (resumePos > 0)
		OutFileStream.open(filename, std::ios::binary | std::ios::app);
	else
		OutFileStream.open(filename, std::ios::binary | std::ios::trunc);

	OutFileStream.seekp(resumePos);

	expectedContentsize -= resumePos;
	uint64_t TotalDataReceived = 0, TotalFileSize = expectedContentsize;

	while (expectedContentsize != 0)
	{
		/* 500MB sized array to limit the intake at once - Pointer so it doesn't go into stack */
		boost::array<Definitions::Byte, Definitions::FileSegmentSize>* ContentArray = new boost::array<Definitions::Byte, Definitions::FileSegmentSize>;

		/* Receive content chuncks */
		size_t ReceivedByteCount = socket.read_some(boost::asio::buffer(*ContentArray));

		/* Total Data received for if the connection gets dropped, to continue the download */
		TotalDataReceived += ReceivedByteCount;

		/* Update Download info, no way to clear the contents so have to reopen the file each time */
		std::ofstream DownloadInfoFileStream("DownloadInfo", std::ios::binary | std::ios::trunc);
		DownloadInfoFileStream << TotalDataReceived;
		DownloadInfoFileStream.close();

		/* Update Content size for new size needed */
		expectedContentsize -= ReceivedByteCount;

		/* Convert to string temporarily to allow for writing into file */
		std::string TempString((char*)ContentArray->data(), ReceivedByteCount);

		/* write content into file */
		OutFileStream.write(TempString.c_str(), TempString.size());

		/* Delete array to free space */
		delete ContentArray;
	}

	OutFileStream.close();

	remove("DownloadInfo");
	/* Read from stream with 500MB sized content segements */
	/* SegementedReceive */
	return;
}