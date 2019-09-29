#pragma once

#include <memory>
#include <vector>
#include <unordered_set>
#include <map>

#include "GlobalVariables.h"
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
#include "PwrC13.h"

#include "TextureManager.h"

#include "rapidxml.hpp"

class MapManager
{

public:

	static void Init();
	static MapManager* Instance();
	~MapManager();

	static void readMap(const char * fileName);
	static void calculateNodesPositions();
	static void removeSkippedObjects();
	void selectObject(float X, float Y, bool(MapManager::*objectChecker)(MapObject&) = nullptr);
	void hideObjects();
	void unhideObjects();
	void deselectObjects();
	void applyMapEditorKeys(Orbit& orbit);
	void saveOverlays();
	void addOverlayAttribute(const char* attribute, const char* value);
	void generatePolygonsFile();
	void loadPolygonsFromFile();

private:
	static std::unique_ptr<char[]> fileToCharReader(const char * fileName);
	static void createNodesMap();
	static void createMapObjectsArray();
	static void addRelationalMapObjectsToArray();
	static void calculateObjectsFinalGeometry();
	static void calculateObjectsBoundingCoordinates();
	static void calculateMapBoundingCoordinates();
	static void applyObjectTag(MapObject& mapObject, rapidxml::xml_node <>* a);
	static void applyOverlays(MapObject& mapObject);

	bool isPasazGrunwaldzkiCheck(MapObject& mapObject);
	bool isGrunwaldzkiCenterCheck(MapObject& mapObject);
	bool isPwrC13Check(MapObject& mapObject);
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
			
			if ((std::is_same<T, Building>::value || std::is_base_of<Building, T>::value) && dynamic_cast<Building*>(mapObjects.back().get())->roofNeeded)
			{
				mapObjects.push_back(std::make_unique<Roof>(Roof(*mapObjects.back().get())));
			}
		}
	}

public:

	bool loadedFromPolygonsFile = false;
	int currentCameraView = 0;

private:

	static float maxX;
	static float minX;
	static float maxY;
	static float minY;

	static double longituteRatio;
	static double latitudeRatio;

	static rapidxml::xml_document <> document;
	static rapidxml::xml_document <> overlays;

	static std::unique_ptr<char[]> overlayContent;

	static std::unordered_set<std::string> skippedTags;
	static std::unordered_set<std::string> temporarySkippedTags;
	static std::unordered_set<std::string> acceptedTags;
	static std::map<std::string, long MapObject::*> tagLongPtrs;
	static std::map<std::string, std::string MapObject::*> tagPtrs;
	static std::vector<std::pair<bool(MapManager::*)(MapObject&), void(MapManager::*)(MapObject&)>> objectDetector;

public:

	static std::map<long long, node> nodes;
	static std::vector<std::unique_ptr<MapObject>> mapObjects;
	static std::vector<std::unique_ptr<MapObject>> raceObjects;
	static std::vector<std::unique_ptr<Object3D>> polygonsObjects;

private:
	static MapManager* _instance;
};