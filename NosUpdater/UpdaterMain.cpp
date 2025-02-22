#include <boost/asio.hpp>

#include <NosUpdate/Test.hpp>

#include <iostream>

int main()
{
	boost::asio::io_context io_context;

	boost::asio::ip::tcp::socket socket(io_context);

	printf("Updater\n");

	printf("Type in hostname: ");
	std::string HostName;
	std::getline(std::cin, HostName);
	if (HostName.empty())
		HostName = "localhost";

	/*
	Connects to the function using `resolver` which resolves the address e.g. (Noscka.com -> 123.123.123.123)
	Host - Hostname/Ip address
	Service - Service(Hostname for ports)/Port number
	*/
	boost::asio::connect(socket, boost::asio::ip::tcp::resolver(io_context).resolve(HostName.c_str(), "8100"));

	printf("Connected to server\n");

	for (;;)
	{
		std::string message;

		std::getline(std::cin, message);
		message += "\n";

		boost::asio::write(socket, boost::asio::buffer(message));
	}

	printf("Press any button to continue"); getchar();
	return 0;
}