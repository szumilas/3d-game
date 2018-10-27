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
	//void createMapObjectsArrayFromRelations();

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
		{ "barrier", &MapObject::barrier },
		{ "leisure", &MapObject::leisure },
		{ "amenity", &MapObject::amenity },
		{ "highway", &MapObject::highway },
		{ "man_made", &MapObject::man_made },
		{ "railway", &MapObject::railway },
	};

	template <typename T>
	void addObject(MapObject& newMapObject)
	{
		mapObjects.push_back(std::make_unique<T>(newMapObject));
	}

	bool isHighlightedObjectCheck(MapObject& mapObject)
	{
		//(mapObject.id == 33286825)
		//	return true;
		//else
			return false;
	}

	bool isStreetCheck(MapObject& mapObject)
	{
		if (!mapObject.area_highway.empty())
			return true;
		else
			return false;
	}

	bool isBuildingCheck(MapObject& mapObject)
	{
		if (!mapObject.building.empty() || !mapObject.building_part.empty())
			return true;
		else
			return false;
	}

	bool isBridgeCheck(MapObject& mapObject)
	{
		if (mapObject.man_made == "bridge")
			return true;
		else
			return false;
	}

	bool isGreenAreaCheck(MapObject& mapObject)
	{
		if (mapObject.landuse == "grass" || mapObject.landuse == "village_green" || mapObject.leisure == "park")
			return true;
		else
			return false;
	}

	bool isBarrierCheck(MapObject& mapObject)
	{
		if (!mapObject.barrier.empty())
			return true;
		else
			return false;
	}

	bool isCommonCheck(MapObject& mapObject)
	{
		if (mapObject.leisure == "common" || mapObject.amenity == "university")
			return true;
		else
			return false;
	}

	bool isParkingCheck(MapObject& mapObject)
	{
		if (mapObject.amenity == "parking")
			return true;
		else
			return false;
	}

	bool isRailwayCheck(MapObject& mapObject)
	{
		if (mapObject.railway == "tram")
			return true;
		else
			return false;
	}

	bool isRiverCheck(MapObject& mapObject)
	{
		if (mapObject.waterway == "river" || mapObject.waterway == "canal")
			return true;
		else
			return false;
	}

	bool isFootwayCheck(MapObject& mapObject)
	{
		if (mapObject.highway == "footway" || mapObject.highway == "path")
			return true;
		else
			return false;
	}

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