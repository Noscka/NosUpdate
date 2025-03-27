#include <NosLib/Boost/WinVersion.hpp>
#include <NosUpdate/Definitions.hpp>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <NosUpdate/Definitions.hpp>
#include <NosLib/Logging.hpp>
#include <NosLib/ErrorHandling.hpp>

#include "Header/TLSClient.hpp"

#include <iostream>

std::string GetServerHostName()
{
	printf("Type in hostname: ");
	std::string HostName;
	std::getline(std::cin, HostName);
	if (HostName.empty())
		HostName = "localhost";

	return HostName;
}

void UpdateTest()
{

	boost::asio::io_context io_context;
	boost::asio::ssl::context ssl_context(boost::asio::ssl::context::sslv23);
	ssl_context.load_verify_file("server.crt");
	ssl_context.set_default_verify_paths();

	#if 1
	std::string hostName = GetServerHostName();
	#else
	std::string hostName = NosUpdate::Definitions::UpdateHostname;
	#endif

	TLSClient updateClient(io_context, ssl_context, hostName, NosUpdate::Definitions::UpdatePort);
	NosLib::Result<void> errorRet = updateClient.Connect();
	NOSLOG_ASSERT(!errorRet, return, NosLib::Logging::Severity::Fatal, "Error Connecting to server: {}", errorRet.ErrorMessage());
	
	errorRet = updateClient.UpdateProgram();
	NOSLOG_ASSERT(!errorRet, return, NosLib::Logging::Severity::Fatal, "Error Updating: {}", errorRet.ErrorMessage());
}

int main()
{
	NosLib::Logging::SetVerboseLevel(NosLib::Logging::Verbose::Debug);

	UpdateTest();

	printf("Press any button to continue"); getchar();
	return 0;
}