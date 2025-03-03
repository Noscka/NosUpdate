#include <NosUpdate/FileNet/FileSend.hpp>

#include <boost/filesystem.hpp>

#include <fstream>

void NosUpdate::SendFile(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& socket)
{
	uint64_t startPos = 0;
	std::string FileAddress = "NosUpdateServer.exe";
	std::ifstream filestream(FileAddress, std::ios::binary);

	uint64_t UnchangedTotalSize = boost::filesystem::file_size(boost::filesystem::path(FileAddress)); /* Will need better name, total size of amount needing to be sent */
	uint64_t TotalSendingSize = UnchangedTotalSize - startPos; /* get total sending size */
	uint64_t FullOperationAmount = (int)(TotalSendingSize / Definitions::FileSegmentSize); /* amount of times server has to send 500MB segements */
	uint64_t BytesLeft = TotalSendingSize % Definitions::FileSegmentSize; /* Amount of bytes left which will get sent seperataly */
	uint64_t CurrentOperationCount = 0; /* Storing current operation count */

	/* while loop until all bytes sent */
	while (TotalSendingSize != 0)
	{
		/* Vector for sending the data gotten from file, is a pointer to not put the object on stack */
		std::vector<Definitions::Byte>* DividedFileContents;

		if (CurrentOperationCount < FullOperationAmount) /* if statement to check if the program should sent 500MB segements */
		{
			/* create a new vector with the segement size (default 500MB unless I changed it) */
			DividedFileContents = new std::vector<Definitions::Byte>(Definitions::FileSegmentSize);

			/* seek the position to read from (in a way, move the file reader pointer to the start of needed bytes) */
			filestream.seekg(CurrentOperationCount * Definitions::FileSegmentSize + startPos);
			/* Read the 500MBs into the vector array */
			filestream.read(reinterpret_cast<char*>(DividedFileContents->data()), Definitions::FileSegmentSize);

			CurrentOperationCount++;
		}
		else /* if false, send the rest of the data which is less then 500MB */
		{
			/* create a new vector with the segement size (default 500MB unless I changed it) */
			DividedFileContents = new std::vector<Definitions::Byte>(BytesLeft);

			/* seek the position to read from (in a way, move the file reader pointer to the start of needed bytes) */
			filestream.seekg((FullOperationAmount * Definitions::FileSegmentSize) + startPos);
			/* Read the bytes left into the vector array */
			filestream.read(reinterpret_cast<char*>(DividedFileContents->data()), BytesLeft);
		}

		/* write the vector into the socket stream for the client. also minus the amount of bytes sent from total */
		TotalSendingSize -= boost::asio::write(socket, boost::asio::buffer(*DividedFileContents));
		/* May not be necessery but just incase, destoy the vector to 100% prevent a memory leak */
		DividedFileContents->~vector();
	}
	/* SendingContents */
}