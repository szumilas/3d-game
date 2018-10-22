#pragma once

#include <memory>
#include <vector>

#include "rapidxml.hpp"

class MapManager
{
	struct node
	{
		long long id;
		double lon;
		double lat;

		float posX;
		float posY;
	};

public:
	void readMap(const char * fileName);
	void calculateNodesPositions();

private:
	std::unique_ptr<char[]> fileToCharReader(const char * fileName);
	void createNodesArray();
	void createBuildingsArray();
	void createStreetsArray();

public:

private:
	double longituteRatio = 69797.5460045862;
	double latitudeRatio = 111220.165038003;

	rapidxml::xml_document <> document;

public:
	std::vector<node> nodes;
	std::vector<std::vector<long long>> buildings;
	std::vector<std::vector<long long>> streets;

};