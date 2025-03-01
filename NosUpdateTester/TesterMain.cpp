#include <NosUpdate/WinVersion.hpp>
#include <NosLib/Logging.hpp>
#include <format>

int main()
{
	NosLib::Logging::SetVerboseLevel(NosLib::Logging::Verbose::Debug);

	std::string exampleString = "I am String";
	NosLib::Logging::CreateLog(NosLib::Logging::Severity::Info, "Normal String: {} | Raw Char String {}", exampleString, "I am Char Array");

	printf("Press any button to continue"); getchar();
	return 0;
}