#include <NosUpdate/Test.hpp>

#include <iostream>

int main()
{
	printf("From Update Server\n");

	NosUpdate::TestString();

	printf("Press any button to continue"); getchar();
	return 0;
}