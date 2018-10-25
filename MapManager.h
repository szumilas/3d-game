#pragma once

#include <memory>
#include <vector>
#include <unordered_set>
#include <map>

#include "MapObject.h"
#include "Street.h"
#include "Building.h"
#include "GreenArea.h"
#include "River.h"
#include "HighlightedObject.h"

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
	//void createMapObjectsArrayFromRelations();

public:

private:
	double longituteRatio = 69797.5460045862;
	double latitudeRatio = 111220.165038003;

	rapidxml::xml_document <> document;


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
		"waterway",
		"height",
		"building:part",
		"min_height",
	};

	std::map<std::string, long MapObject::*> tagLongPtrs{

	};

	std::map<std::string, std::string MapObject::*> tagStringPtrs{
		{ "area:highway", &MapObject::area_highway },
		{ "building", &MapObject::building },
		{ "landuse", &MapObject::landuse },
		{ "waterway", &MapObject::waterway },
		{ "height", &MapObject::height },
		{ "building:part", &MapObject::building_part },
		{ "min_height", &MapObject::min_height },
	};

public:
	std::map<long long, node> nodes;

	std::vector<std::unique_ptr<MapObject>> mapObjects;

};