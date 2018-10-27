#pragma once

#include <memory>
#include <vector>
#include <unordered_set>
#include <map>

#include "GlobalStructures.h"

#include "MapObject.h"
#include "Street.h"
#include "Building.h"
#include "GreenArea.h"
#include "River.h"
#include "HighlightedObject.h"
#include "Barrier.h"
#include "Common.h"
#include "Parking.h"
#include "StreetLamp.h"
#include "Bridge.h"
#include "Railway.h"
#include "Footway.h"

#include "rapidxml.hpp"

class MapManager
{

public:
	void readMap(const char * fileName);
	void calculateNodesPositions();

private:
	std::unique_ptr<char[]> fileToCharReader(const char * fileName);
	void createNodesMap();
	void createMapObjectsArray();

	bool isHighlightedObjectCheck(MapObject& mapObject);
	bool isStreetCheck(MapObject& mapObject);
	bool isBuildingCheck(MapObject& mapObject);
	bool isBridgeCheck(MapObject& mapObject);
	bool isGreenAreaCheck(MapObject& mapObject);
	bool isBarrierCheck(MapObject& mapObject);
	bool isCommonCheck(MapObject& mapObject);
	bool isParkingCheck(MapObject& mapObject);
	bool isRailwayCheck(MapObject& mapObject);
	bool isRiverCheck(MapObject& mapObject);
	bool isFootwayCheck(MapObject& mapObject);

	template <typename T>
	void addObject(MapObject& newMapObject)
	{
		mapObjects.push_back(std::make_unique<T>(newMapObject));
	}

public:


private:
	double longituteRatio = 69797.5460045862;
	double latitudeRatio = 111220.165038003;

	rapidxml::xml_document <> document;


	std::unordered_set<std::string> skippedTags{
		/*"cycleway:right",
		"description",
		"maxspeed",
		"name:etymology:wikidata",
		"source:maxspeed",*/
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
		"barrier",
		"leisure",
		"amenity",
		"highway",
		"man_made",
		"railway",
		"colour",
		"building:colour",
	};

	std::map<std::string, long MapObject::*> tagLongPtrs{

	};

	std::map<std::string, std::string MapObject::*> tagPtrs{
		{ "area:highway", &MapObject::area_highway },
		{ "building", &MapObject::building },
		{ "landuse", &MapObject::landuse },
		{ "waterway", &MapObject::waterway },
		{ "height", &MapObject::height },
		{ "building:part", &MapObject::building_part },
		{ "min_height", &MapObject::min_height },
		{ "barrier", &MapObject::barrier },
		{ "leisure", &MapObject::leisure },
		{ "amenity", &MapObject::amenity },
		{ "highway", &MapObject::highway },
		{ "man_made", &MapObject::man_made },
		{ "railway", &MapObject::railway },
		{ "colour", &MapObject::colour },
		{ "building:colour", &MapObject::colour },
	};

	std::vector<std::pair<bool(MapManager::*)(MapObject&), void(MapManager::*)(MapObject&)>> objectDetector
	{
		{ &MapManager::isHighlightedObjectCheck, &MapManager::addObject<HighlightedObject>},
		{ &MapManager::isStreetCheck, &MapManager::addObject<Street>},
		{ &MapManager::isBuildingCheck, &MapManager::addObject<Building>},
		{ &MapManager::isRiverCheck, &MapManager::addObject<River> },
		{ &MapManager::isBridgeCheck, &MapManager::addObject<Bridge>},
		{ &MapManager::isGreenAreaCheck, &MapManager::addObject<GreenArea>},
		{ &MapManager::isBarrierCheck, &MapManager::addObject<Barrier>},
		{ &MapManager::isCommonCheck, &MapManager::addObject<Common>},
		{ &MapManager::isParkingCheck, &MapManager::addObject<Parking>},
		{ &MapManager::isRailwayCheck, &MapManager::addObject<Railway>},
		{ &MapManager::isFootwayCheck, &MapManager::addObject<Footway>},
	};

public:

	std::map<long long, node> nodes;
	std::vector<std::unique_ptr<MapObject>> mapObjects;

};