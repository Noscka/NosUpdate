#include <NosUpdate/Test.hpp>

#include <NosLib/HttpClient.hpp>

#include <iostream>

void NosUpdate::TestString()
{
	using NosClient = NosLib::HttpClient;

	NosClient::ptr testClient = NosClient::MakeClient("https://dicksuckfor5bucks.com");
	httplib::Result res = testClient->Get("/");

	printf("dicksuckfor5bucks.com reply: %s\n", res->body.c_str());
	printf("From Update Library\n");
}