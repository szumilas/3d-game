#pragma once

#include <memory>

#include "rapidxml.hpp"

class MapManager
{
public:
	void readMap(const char * fileName);

private:
	std::unique_ptr<char[]> fileToCharReader(const char * fileName);

public:

private:
	rapidxml::xml_document <> document;

};