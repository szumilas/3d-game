#include <fstream>
#include <iostream>

#include "MapManager.h"

#include "rapidxml_print.hpp"


std::unique_ptr<char[]> MapManager::fileToCharReader(const char * fileName)
{
	std::fstream file(fileName);
	if (!file)
	{
		throw std::runtime_error("File '" + std::string(fileName) + " can't be opened!");
	}
	file.seekg(0, file.end);
	const auto fileSize = file.tellg();
	file.seekg(0);

	auto fileContext = std::make_unique<char[]>(fileSize);
	file.read(fileContext.get(), fileSize);

	return fileContext;
}

void MapManager::readMap(const char * fileName)
{
	auto fileContent = fileToCharReader("xml.txt");

	try
	{
		document.parse < 0 >(fileContent.get()); // (2)
	}
	catch (const rapidxml::parse_error & e)
	{
		//TODO: errors
	}

	rapidxml::print(std::cout, document, 0);

}