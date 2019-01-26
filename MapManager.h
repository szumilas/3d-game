#pragma once

#include <memory>
#include <vector>
#include <unordered_set>
#include <map>

#include "enum.h"
#include "GlobalStructures.h"

#include "Orbit.h"
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
#include "Tree.h"
#include "Roof.h"
#include "Water.h"
#include "Riverbank.h"
#include "BusShelter.h"
#include "Crossing.h"
#include "PasazGrunwaldzki.h"
#include "GrunwaldzkiCenter.h"
#include "Sedesowiec.h"

#include "TextureManager.h"

#include "rapidxml.hpp"

class MapManager
{

public:
	void setTextures(TextureManager* textureManager) { MapManager::textureManager = textureManager; };
	void readMap(const char * fileName);
	void calculateNodesPositions();
	void removeSkippedObjects();
	void selectObject(float X, float Y, bool(MapManager::*objectChecker)(MapObject&) = nullptr);
	void hideObjects();
	void unhideObjects();
	void deselectObjects();
	void applyMapEditorKeys(Orbit& orbit);
	void saveOverlays();
	void addOverlayAttribute(const char* attribute, const char* value);

private:
	std::unique_ptr<char[]> fileToCharReader(const char * fileName);
	void createNodesMap();
	void createMapObjectsArray();
	void addRelationalMapObjectsToArray();
	void calculateObjectsFinalGeometry();
	void calculateObjectsBoundingCoordinates();
	void calculateMapBoundingCoordinates();
	void applyObjectTag(MapObject& mapObject, rapidxml::xml_node <>* a);
	void applyOverlays(MapObject& mapObject);

	bool isPasazGrunwaldzkiCheck(MapObject& mapObject);
	bool isGrunwaldzkiCenterCheck(MapObject& mapObject);
	bool isSedesowiecCheck(MapObject& mapObject);
	bool isBusShelterCheck(MapObject& mapObject);
	bool isCrossingCheck(MapObject& mapObject);
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
	bool isWater(MapObject& mapObject);
	bool isRiverbank(MapObject& mapObject);

	template <typename T>
	void addObject(MapObject& newMapObject)
	{
		if (newMapObject._skip != "yes")
		{
			mapObjects.push_back(std::make_unique<T>(newMapObject));
			newMapObject._height = mapObjects.back()->_height;
			if (std::is_same<T, Building>::value || std::is_same<T, PasazGrunwaldzki>::value || std::is_same<T, GrunwaldzkiCenter>::value || std::is_same<T, Sedesowiec>::value)
			{
				//mapObjects.back()->setTextureId(textureManager->textureIds.at(Texture::Te));
			
					mapObjects.push_back(std::make_unique<Roof>(Roof(newMapObject)));

				//mapObjects.back()->setTextureId(textureManager->textureIds[1]);
			}
		}
	}

public:


private:

	float maxX = -10000000.0f;
	float minX =  10000000.0f;
	float maxY = -10000000.0f;
	float minY =  10000000.0f;

	TextureManager* textureManager;

	double longituteRatio = 69797.5460045862;
	double latitudeRatio = 111220.165038003;

	rapidxml::xml_document <> document;
	rapidxml::xml_document <> overlays;

	std::unique_ptr<char[]> overlayContent;

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
		"roof:shape",
		"_skip",
		"natural",
		"width",
		"type",
		"footway",
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
		{ "roof:shape", &MapObject::roof_shape },
		{ "_skip", &MapObject::_skip },
		{ "natural", &MapObject::natural },
		{ "width", &MapObject::width },
		{ "type", &MapObject::type },
		{ "footway", &MapObject::footway },
	};

	std::vector<std::pair<bool(MapManager::*)(MapObject&), void(MapManager::*)(MapObject&)>> objectDetector
	{
		{ &MapManager::isPasazGrunwaldzkiCheck, &MapManager::addObject<PasazGrunwaldzki> },
		{ &MapManager::isGrunwaldzkiCenterCheck, &MapManager::addObject<GrunwaldzkiCenter> },
		{ &MapManager::isSedesowiecCheck, &MapManager::addObject<Sedesowiec> },
		{ &MapManager::isBusShelterCheck, &MapManager::addObject<BusShelter> },
		{ &MapManager::isCrossingCheck, &MapManager::addObject<Crossing> },
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
		{ &MapManager::isWater, &MapManager::addObject<Water>},
		{ &MapManager::isRiverbank, &MapManager::addObject<Riverbank>},
	};

public:

	std::map<long long, node> nodes;
	std::vector<std::unique_ptr<MapObject>> mapObjects;

};