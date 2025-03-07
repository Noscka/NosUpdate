#include <NosUpdate/WinVersion.hpp>
#include <NosLib/Logging.hpp>

#include <format>
#include <iostream>
#include <fstream>

constexpr uint64_t KILOBYTE(1000);
constexpr uint64_t MEGABYTE(1000 * KILOBYTE);
constexpr uint64_t GIGABYTE(1000 * MEGABYTE);

enum class ByteUnit : uint8_t
{
	KiloByte,
	MegaByte,
	GigaByte
};

std::string CreateLine(const uint64_t& normalNumber)
{
	const uint16_t digitCount = 20;
	std::string line = std::format("{}\n", normalNumber);
	line.insert(0, digitCount - line.size(), '0');
	return line;
}

void CreateData(const uint64_t& size)
{
	uint64_t segmentSize = 256 * KILOBYTE;

	std::ofstream dataFStream("TestData.txt", std::ios::binary);

	std::string data = CreateLine(size);
	dataFStream.write(data.c_str(), data.size());

	for (uint64_t i = 1; i <= (size/20); i++)
	{
		data = CreateLine(i);
		dataFStream.write(data.c_str(), data.size());
	}
}

ByteUnit GetByteUnit(const std::string& str)
{
	if (str == "KB") return ByteUnit::KiloByte;
	if (str == "MB") return ByteUnit::MegaByte;
	if (str == "GB") return ByteUnit::GigaByte;
}

uint64_t GetBytes(const ByteUnit& unit)
{
	switch (unit)
	{
	case ByteUnit::KiloByte:
		return KILOBYTE;
	case ByteUnit::MegaByte:
		return MEGABYTE;
	case ByteUnit::GigaByte:
		return GIGABYTE;
	}
}

uint64_t GetSize()
{
	std::string sizeStr;
	printf("Enter a size to make[KB,MB,GB]: ");
	getline(std::cin, sizeStr);

	size_t sizeStrSize = sizeStr.size();
	std::string sizeUnit = sizeStr.substr(sizeStrSize-2, 2);
	std::string sizeNumber = sizeStr.substr(0, sizeStrSize - 2);

	ByteUnit byteUnit = GetByteUnit(sizeUnit);

	float sizeFloat = std::stof(sizeNumber);

	return sizeFloat*GetBytes(byteUnit);
}

int main()
{
	NosLib::Logging::SetVerboseLevel(NosLib::Logging::Verbose::Debug);
	
	CreateData(GetSize());

	printf("Press any button to continue"); getchar();
	return 0;
}