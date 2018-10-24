#pragma once

#include <memory>
#include <vector>
#include <unordered_set>
#include <map>

#include "MapObject.h"
#include "Street.h"
#include "Building.h"
#include "GreenArea.h"

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
	void createNodesMap();
	void createMapObjectsArray();

public:

private:
	double longituteRatio = 69797.5460045862;
	double latitudeRatio = 111220.165038003;

	rapidxml::xml_document <> document;

	std::vector<MapObject> mapObjects;

	std::unordered_set<std::string> skippedTags{
		"cycleway:right",
		"description",
		"maxspeed",
		"name:etymology:wikidata",
		"source:maxspeed",
	};

	std::unordered_set<std::string> temporarySkippedTags{
	};

	std::unordered_set<std::string> acceptedTags{
		"area:highway",
		"building",
		"landuse",
	};

	std::map<std::string, long MapObject::*> tagLongPtrs{

	};

	std::map<std::string, std::string MapObject::*> tagStringPtrs{
		{ "area:highway", &MapObject::area_highway },
		{ "building", &MapObject::building },
		{ "landuse", &MapObject::landuse },
	};

public:
	std::map<long long, node> nodes;

	std::vector<Street> streets;
	std::vector<Building> buildings;
	std::vector<GreenArea> greenAreas;

};