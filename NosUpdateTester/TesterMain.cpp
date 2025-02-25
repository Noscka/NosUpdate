#include <NosLib/HttpClient.hpp>

#include <iostream>

int main()
{
	using NosClient = NosLib::HttpClient;

	printf("Tester Application\n");

	NosClient::ptr testClient = NosClient::MakeClient("https://dicksuckfor5bucks.com");
	httplib::Result res = testClient->Get("/");

	printf("dicksuckfor5bucks.com reply: %s\n", res->body.c_str());
	printf("From Update Library\n");

	printf("Press any button to continue"); getchar();
	return 0;
}