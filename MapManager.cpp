#include <fstream>
#include <iostream>
#include <string>
#include "PointInsidePolygonDetector.h"

#include "CameraManager.h"
#include "MapManager.h"
#include "Skybox.h"
#include "RelationalMapObject.h"
#include "MapContainer.h"

#include "rapidxml_print.hpp"

#include "KeyboardManager.h"

MapManager* MapManager::_instance;

rapidxml::xml_document <> MapManager::document;
rapidxml::xml_document <> MapManager::overlays;
rapidxml::xml_document <> MapManager::extraObjects;
std::unique_ptr<char[]> MapManager::overlayContent;
std::unique_ptr<char[]> MapManager::extraOjectsContent;

float MapManager::maxX = -10000000.0f;
float MapManager::minX = 10000000.0f;
float MapManager::maxY = -10000000.0f;
float MapManager::minY = 10000000.0f;
const double MapManager::longituteRatio = 69797.5460045862;
const double MapManager::latitudeRatio = 111220.165038003;
const double MapManager::minLat = 51.112663;
const double MapManager::minLon = 17.056106;
std::string MapManager::mapPath;
long long MapManager::lastExtraNodeId;
long long MapManager::lastExtraObjectId;

std::map<long long, node> MapManager::nodes;
std::map<long long, node> MapManager::extraNodes;
std::vector<std::unique_ptr<MapObject>> MapManager::mapObjects;
std::vector<std::unique_ptr<MapObject>> MapManager::raceObjects;
std::vector<std::unique_ptr<Object3D>> MapManager::polygonsObjects;

std::vector<std::pair<bool(MapManager::*)(MapObject&), void(MapManager::*)(MapObject&)>> MapManager::objectDetector
{
	{ &MapManager::isPasazGrunwaldzkiCheck, &MapManager::addObject<PasazGrunwaldzki> },
	{ &MapManager::isGrunwaldzkiCenterCheck, &MapManager::addObject<GrunwaldzkiCenter> },
	{ &MapManager::isPwrC13Check, &MapManager::addObject<PwrC13> },
	{ &MapManager::isSedesowiecCheck, &MapManager::addObject<Sedesowiec> },
	{ &MapManager::isBusShelterCheck, &MapManager::addObject<BusShelter> },
	{ &MapManager::isCrossingCheck, &MapManager::addObject<Crossing> },
	{ &MapManager::isHighlightedObjectCheck, &MapManager::addObject<HighlightedObject> },
	{ &MapManager::isStreetCheck, &MapManager::addObject<Street> },
	{ &MapManager::isBuildingCheck, &MapManager::addObject<Building> },
	{ &MapManager::isRiverCheck, &MapManager::addObject<River> },
	{ &MapManager::isBridgeCheck, &MapManager::addObject<Bridge> },
	{ &MapManager::isGreenAreaCheck, &MapManager::addObject<GreenArea> },
	{ &MapManager::isCustomAreaCheck, &MapManager::addObject<CustomArea> },
	{ &MapManager::isBarrierCheck, &MapManager::addObject<Barrier> },
	{ &MapManager::isCommonCheck, &MapManager::addObject<Common> },
	{ &MapManager::isParkingCheck, &MapManager::addObject<Parking> },
	{ &MapManager::isRailwayCheck, &MapManager::addObject<Railway> },
	{ &MapManager::isFootwayCheck, &MapManager::addObject<Footway> },
	{ &MapManager::isWater, &MapManager::addObject<Water> },
	{ &MapManager::isRiverbank, &MapManager::addObject<Riverbank> },
	{ &MapManager::isRamp, &MapManager::addObject<Ramp> },
};

std::unordered_set<std::string> MapManager::skippedTags{
	/*"cycleway:right",
	"description",
	"maxspeed",
	"name:etymology:wikidata",
	"source:maxspeed",*/
};

std::unordered_set<std::string> MapManager::temporarySkippedTags{
};

std::unordered_set<std::string> MapManager::acceptedTags{
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
	"_custom_texture",
	"_custom_object",
	"_custom_area",
};

std::map<std::string, long MapObject::*> MapManager::tagLongPtrs{

};

std::map<std::string, std::string MapObject::*> MapManager::tagPtrs{
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
	{ "_custom_texture", &MapObject::_custom_texture },
	{ "_custom_object", &MapObject::_custom_object },
	{ "_custom_area", &MapObject::_custom_area },
};

void printAttributes(rapidxml::xml_node <>* node)
{
	for (rapidxml::xml_attribute <>* a = node->first_attribute(); a; a = a->next_attribute())
	{
		std::cout << a->name() << "' = '" << a->value() << "'\t";
	}
}

void MapManager::Init()
{
	_instance = new MapManager;

	//mapManager.readMap("szczytnicka.osm");
	//mapManager.readMap("szczytnickaB4.osm");
	//mapManager.readMap("szczytnickaB.osm");
	//mapManager.readMap("map.osm");
	//mapManager.readMap("grunwald.osm");
	//mapManager.readMap("parkCheck.osm");

	std::cout << "reading map...";
#ifdef _DEBUG
	Instance()->readMap("huge.osm");
	//Instance()->readMap("grunwaldWithRiver.osm");
	//Instance()->readMap("huge.osm");
#else
	//Instance()->readMap("trees2.osm");
	//Instance()->readMap("grunwaldWithRiver.osm");
	Instance()->readMap("huge.osm");
#endif
	std::cout << "...finished\n";
	//mapManager.readMap("singlebuilding.osm");
	//mapManager.readMap("walls.osm");

	//mapManager.readMap("streetDetail.osm");
	//mapManager.readMap("lake.osm");
	//mapManager.readMap("A1.osm");
	//mapManager.readMap("d1only.osm");
	//mapManager.readMap("river.osm");
	//mapManager.readMap("curie3.osm");
	//mapManager.readMap("shelter.osm");
	//mapManager.readMap("stairs.osm");
	//mapManager.readMap("pasaz.osm");
	//mapManager.readMap("sedesowce.osm");
	//mapManager.readMap("grunwaldzki.osm");
	//mapManager.readMap("c13.osm");

	//mapManager.generatePolygonsFile();
	//mapManager.loadPolygonsFromFile();
	//mapContainer.loadWorldIntoBuckets(&mapManager.polygonsObjects);
}

MapManager* MapManager::Instance()
{
	return _instance;
}

MapManager::~MapManager()
{
	delete _instance;
}

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

void MapManager::rereadMap()
{
	readMap(mapPath.c_str());
	MapContainer::Instance()->loadWorldIntoSections(MapManager::Instance()->mapObjects);
}

void MapManager::readMap(const char * fileName)
{
	mapPath = fileName;

	mapObjects.clear();
	document.clear();
	overlays.clear();
	extraObjects.clear();
	nodes.clear();
	maxX = -10000000.0f;
	minX = 10000000.0f;
	maxY = -10000000.0f;
	minY = 10000000.0f;

	auto fileContent = fileToCharReader(fileName);

	try
	{
		document.parse < 0 >(fileContent.get()); // (2)
	}
	catch (const rapidxml::parse_error & e)
	{
		//TODO: errors
		std::cout << "ERROR while parsing document\n";
		std::cout << '\a';
		Sleep(2000);
	}
	
	overlayContent = fileToCharReader("worldOverlays.xml");

	try
	{
		overlays.parse < 0 >(overlayContent.get()); // (2)
	}
	catch (const rapidxml::parse_error & e)
	{
		//TODO: errors
		std::cout << "ERROR while parsing overlays\n";
		std::cout << '\a';
		Sleep(2000);
	}

	extraOjectsContent = fileToCharReader("extraObjects.xml");

	try
	{
		extraObjects.parse < 0 >(extraOjectsContent.get()); // (2)
	}
	catch (const rapidxml::parse_error & e)
	{
		//TODO: errors
		std::cout << "ERROR while parsing extraObjects\n";
		std::cout << '\a';
		Sleep(2000);
	}

	
	createNodesMap();
	createMapObjectsArray();
	calculateNodesPositions();
	removeSkippedObjects();

	
	calculateObjectsFinalGeometry();
	calculateObjectsBoundingCoordinates();
	calculateMapBoundingCoordinates();

	mapObjects.push_back(std::make_unique<Skybox>(Skybox(minX, maxX, minY, maxY)));
}

void MapManager::saveOverlays()
{
	std::filebuf fb;
	fb.open("worldOverlays.xml", std::ios::out);
	std::ostream os(&fb);
	rapidxml::print(os, overlays);
	fb.close();
}

void MapManager::createNodesMap()
{
	std::vector<rapidxml::xml_node <>*> documents{ document.first_node(), extraObjects.first_node() };

	long long lastExtraNodeId = 0;

	for (auto& nodeCeo : documents)
	{
		for (rapidxml::xml_node <>* manager = nodeCeo->first_node(); manager; manager = manager->next_sibling()) // (3)
		{
			if (!strcmp(manager->name(), "node"))
			{
				node newNode;

				for (rapidxml::xml_attribute <>* a = manager->first_attribute(); a; a = a->next_attribute())
				{
					if (!strcmp(a->name(), "id"))
					{
						newNode.id = std::stoll(a->value());
						if (newNode.id < 0 && newNode.id < lastExtraNodeId)
							lastExtraNodeId = newNode.id;
					}
					else if (!strcmp(a->name(), "lat"))
					{
						newNode.lat = std::stod(a->value());
					}
					else if (!strcmp(a->name(), "lon"))
					{
						newNode.lon = std::stod(a->value());
					}
				}

				for (rapidxml::xml_node <>* a = manager->first_node(); a; a = a->next_sibling())
				{
					std::string currentTag;
					std::string currentTagValue;

					if (!strcmp(a->name(), "tag"))
					{
						for (rapidxml::xml_attribute <>* b = a->first_attribute(); b; b = b->next_attribute())
						{
							if (!strcmp(b->name(), "k")) //attribute name
							{
								if (acceptedTags.count(b->value()))
								{
									currentTag = b->value();
								}
							}
							else if (!strcmp(b->name(), "v")) //attribute value
							{
								currentTagValue = b->value();
							}
							else
							{
								break;
							}
						}
					}

					if (currentTag == "highway" && currentTagValue == "street_lamp")
					{
						mapObjects.push_back(std::make_unique<StreetLamp>(newNode.id));
					}
					else if (currentTag == "natural" && currentTagValue == "tree")
					{
						mapObjects.push_back(std::make_unique<Tree>(newNode.id));
					}
				}

				if (newNode.id && newNode.lat && newNode.lon)
				{
					nodes[newNode.id] = newNode;
				}
				//std::cout << std::endl;
			}
		}
	}

	MapManager::lastExtraNodeId = lastExtraNodeId - 1;
}

void MapManager::createMapObjectsArray()
{
	std::vector<rapidxml::xml_node <>*> documents{ document.first_node(), extraObjects.first_node() };

	long long lastExtraObjectId = -100000;

	for (auto& nodeCeo : documents)
	{
		for (rapidxml::xml_node <>* manager = nodeCeo->first_node(); manager; manager = manager->next_sibling()) // (3)
		{
			if (!strcmp(manager->name(), "way"))
			{
				long long id = std::stoll(manager->first_attribute()->value());
				MapObject mapObject(id);

				if (id < 0 && id < lastExtraObjectId)
					lastExtraObjectId = id;

				for (rapidxml::xml_node <>* a = manager->first_node(); a; a = a->next_sibling())
				{
					if (!strcmp(a->name(), "nd"))
					{
						for (rapidxml::xml_attribute <>* b = a->first_attribute(); b; b = b->next_attribute())
						{
							if (!strcmp(b->name(), "ref"))
							{
								mapObject.refs.push_back(std::stoll(b->value()));
								break;
							}
						}
					}
					else if (!strcmp(a->name(), "tag"))
					{
						applyObjectTag(mapObject, a);
					}
				}

				applyOverlays(mapObject);

				mapObject.applyKnownValues();

				for (auto& check : objectDetector)
				{
					if ((Instance()->*check.first)(mapObject))
					{
						(Instance()->*check.second)(mapObject);
						break;
					}
				}
			}
		}
	}

	MapManager::lastExtraObjectId = lastExtraObjectId - 1;
}

void MapManager::addRelationalMapObjectsToArray()
{
	std::vector<RelationalMapObject> relationalMapObjects;
	std::unordered_set<long long> usedWays;

	rapidxml::xml_node <>* nodeCeo = document.first_node();
	for (rapidxml::xml_node <>* manager = nodeCeo->first_node(); manager; manager = manager->next_sibling()) // (3)
	{
		if (!strcmp(manager->name(), "relation"))
		{
			RelationalMapObject relationalMapObject(std::stoll(manager->first_attribute()->value()));

			for (rapidxml::xml_node <>* a = manager->first_node(); a; a = a->next_sibling())
			{

				if (!strcmp(a->name(), "member"))
				{
					std::string type;
					std::string ref;
					std::string role;
					for (rapidxml::xml_attribute <>* b = a->first_attribute(); b; b = b->next_attribute())
					{
						if (!strcmp(b->name(), "type"))
						{
							type = b->value();
						}
						else if (!strcmp(b->name(), "ref"))
						{
							ref = b->value();
						}
						else if (!strcmp(b->name(), "role"))
						{
							role = b->value();
						}
					}

					if (type == "way" && !ref.empty() && !role.empty())
					{
						relationalMapObject.addMember(stoll(ref), role);
						usedWays.emplace(stoll(ref));
					}
				}
				else if (!strcmp(a->name(), "tag"))
				{
					applyObjectTag(relationalMapObject, a);
				}
			}


			applyOverlays(relationalMapObject);

			relationalMapObject.applyKnownValues();

			relationalMapObjects.push_back(std::move(relationalMapObject));
		}
	}

	std::map<long long, std::vector<long long>> ways;

	nodeCeo = document.first_node();
	for (rapidxml::xml_node <>* manager = nodeCeo->first_node(); manager; manager = manager->next_sibling()) // (3)
	{
		if (!strcmp(manager->name(), "way"))
		{
			long long wayId = std::stoll(manager->first_attribute()->value());
			if (usedWays.find(wayId) == usedWays.end())
				continue;

			std::vector<long long> nodes;

			for (rapidxml::xml_node <>* a = manager->first_node(); a; a = a->next_sibling())
			{
				if (!strcmp(a->name(), "nd"))
				{
					for (rapidxml::xml_attribute <>* b = a->first_attribute(); b; b = b->next_attribute())
					{
						if (!strcmp(b->name(), "ref"))
						{
							nodes.push_back(std::stoll(b->value()));
							break;
						}
					}
				}
			}

			ways.insert({ wayId, nodes });
		}
	}

	for (auto& relationalMapObject : relationalMapObjects)
	{
		relationalMapObject.createNodeGeometry(ways, nodes);

		if (!relationalMapObject.points.empty())
		{
			for (auto& check : objectDetector)
			{
				if ((Instance()->*check.first)(relationalMapObject))
				{
					(Instance()->*check.second)(relationalMapObject);
					break;
				}
			}
		}

	}
}

void MapManager::applyObjectTag(MapObject& mapObject, rapidxml::xml_node <>* a)
{
	std::string currentTag;
	std::string currentTagValue;
	bool skipTag = false;

	for (rapidxml::xml_attribute <>* b = a->first_attribute(); b; b = b->next_attribute())
	{
		if (!strcmp(b->name(), "k")) //attribute name
		{
			if (skippedTags.count(b->value()) || temporarySkippedTags.count(b->value()))
			{
				skipTag = true;
				break;
			}
			else if (acceptedTags.count(b->value()))
			{
				currentTag = b->value();
			}
			else
			{
				//assert(!"unknown tag");
			}

		}
		else if (!strcmp(b->name(), "v")) //attribute value
		{
			currentTagValue = b->value();
		}
		else
		{
			break;
		}
	}
	if (!skipTag)
	{
		auto mapElement = tagLongPtrs.find(currentTag);
		if (mapElement != tagLongPtrs.end())
		{
			mapObject.*mapElement->second = 5;
		}
		else
		{
			auto mapElement = tagPtrs.find(currentTag);
			if (mapElement != tagPtrs.end())
			{
				mapObject.*mapElement->second = currentTagValue;
			}
		}
	}
}

/*void MapManager::createMapObjectsArrayFromRelations()
{
	rapidxml::xml_node <>* nodeCeo = document.first_node();
	for (rapidxml::xml_node <>* manager = nodeCeo->first_node(); manager; manager = manager->next_sibling()) // (3)
	{
		if (!strcmp(manager->name(), "relation"))
		{

			for (rapidxml::xml_node <>* a = manager->first_node(); a; a = a->next_sibling())
			{
				long long ref = false;
				std::string type;
				std::string role;

				std::string currentTag;
				std::string currentTagValue;
				bool skipTag = false;

				if (!strcmp(a->name(), "member"))
				{
					for (rapidxml::xml_attribute <>* b = a->first_attribute(); b; b = b->next_attribute())
					{
						if (!strcmp(b->name(), "ref"))
						{
							ref = std::stoll(b->value());
						}
						else if (!strcmp(b->name(), "type"))
						{
							type = b->value();
						}
						else if (!strcmp(b->name(), "role"))
						{
							role = b->value();
						}
					}

					if (type == "way" && role == "outer")
					{

					}
				}
			}
		}
	}
}*/

Point MapManager::ConvertCoordinatesToLocalWorld(Point p)
{
	p.x = static_cast<float>((p.x - minLon) * longituteRatio);
	p.y = static_cast<float>((p.y - minLat) * latitudeRatio);
	return p;
}

void MapManager::calculateNodesPositions()
{
	for (auto it = nodes.begin(); it != nodes.end(); ++it)
	{
		it->second.posX = static_cast<float>((it->second.lon - minLon) * longituteRatio);
		it->second.posY = static_cast<float>((it->second.lat - minLat) * latitudeRatio);
	}

	addRelationalMapObjectsToArray();

	long long polygonsCount = 0;
	long long edgesCount = 0;
	int q = 0;
	for (auto& mapObject : mapObjects)
	{
		mapObject->calculateXYfromRef(nodes);
		polygonsCount += mapObject->polygonsCount;
		edgesCount += mapObject->edgesCount;

		q++;
	}
}

node MapManager::addNewExtraNode(Point& p)
{
	node newExtraNode;
	newExtraNode.id = lastExtraNodeId;
	newExtraNode.posX = p.x;
	newExtraNode.posY = p.y;
	newExtraNode.lon = p.x / longituteRatio + minLon;
	newExtraNode.lat = p.y / latitudeRatio + minLat;

	extraNodes.insert({ newExtraNode.id, newExtraNode });

	lastExtraNodeId--;
	return newExtraNode;
}

void MapManager::removeExtraNode(long long ref)
{
	for (auto it = extraNodes.begin(); it != extraNodes.end(); it++)
	{
		if (it->first == ref)
		{
			extraNodes.erase(it);
			break;
		}
	}
}

void MapManager::removeSkippedObjects()
{
	mapObjects.erase(
		std::remove_if(mapObjects.begin(), mapObjects.end(), [&](const std::unique_ptr<MapObject>& a) {return a->_skip == "yes"; }),
		mapObjects.end());
}

bool MapManager::isPasazGrunwaldzkiCheck(MapObject& mapObject)
{
	if (mapObject.getId() == 101195833 || mapObject.getId() == 101212793 || mapObject.getId() == 101213837 || mapObject.getId() == 101189502)
	{
		return true; 
	}
	else
		return false;
}

bool MapManager::isGrunwaldzkiCenterCheck(MapObject& mapObject)
{
	if (mapObject.getId() == 101195194 || mapObject.getId() == 101212152 || mapObject.getId() == 101207075 || mapObject.getId() == 101215218 ||
		mapObject.getId() == 1442935 || mapObject.getId() == 101188144 || mapObject.getId() == 101191558)
	{
		return true; 
	}
	else
		return false;
}

bool MapManager::isPwrC13Check(MapObject& mapObject)
{
	if (mapObject.getId() == 353785323 || mapObject.getId() == 479882765 || mapObject.getId() == 479882766)
	{
		return true; 
	}
	else
		return false;
}

bool MapManager::isSedesowiecCheck(MapObject& mapObject)
{
	if (mapObject.getId() == 101212302 || mapObject.getId() == 402873616 || mapObject.getId() == 403442403 || mapObject.getId() == 440403931 || mapObject.getId() == 440403932 || mapObject.getId() == 440403933 ||
		mapObject.getId() == 381819562 || mapObject.getId() == 402873614 || mapObject.getId() == 403442401 || mapObject.getId() == 440403922 || mapObject.getId() == 440403923 || mapObject.getId() == 440403924)
	{
		return true; 
	}
	else
		return false;
}

bool MapManager::isBusShelterCheck(MapObject& mapObject)
{
	if (mapObject.building == "yes" && mapObject.amenity == "shelter")
	{
		return true; 
	}
	else
		return false;
}

bool MapManager::isCrossingCheck(MapObject& mapObject)
{
	if (mapObject.footway == "crossing")
	{
		return true; 
	}
	else
		return false;
}

bool MapManager::isHighlightedObjectCheck(MapObject& mapObject)
{
	if(mapObject.getId() == 101212380ll)
		return true;
	else
		return false;
}

bool MapManager::isStreetCheck(MapObject& mapObject)
{
	if (!mapObject.area_highway.empty())
		return true;
	else
		return false;
}

bool MapManager::isBuildingCheck(MapObject& mapObject)
{
	if (mapObject.building != "roof_generated" && (!mapObject.building.empty() || !mapObject.building_part.empty()))
		return true;
	else
		return false;
}

bool MapManager::isBridgeCheck(MapObject& mapObject)
{
	if (mapObject.man_made == "bridge")
		return true;
	else
		return false;
}

bool MapManager::isGreenAreaCheck(MapObject& mapObject)
{
	if (mapObject.landuse == "grass" || mapObject.landuse == "village_green" || mapObject.leisure == "park")
		return true;
	else
		return false;
}

bool MapManager::isCustomAreaCheck(MapObject& mapObject)
{
	if (mapObject._custom_area == "yes")
		return true;
	else
		return false;
}

bool MapManager::isBarrierCheck(MapObject& mapObject)
{
	if (!mapObject.barrier.empty())
		return true;
	else
		return false;
}

bool MapManager::isCommonCheck(MapObject& mapObject)
{
	if (mapObject.leisure == "common" || mapObject.amenity == "university")
		return true;
	else
		return false;
}

bool MapManager::isParkingCheck(MapObject& mapObject)
{
	if (mapObject.amenity == "parking")
		return true;
	else
		return false;
}

bool MapManager::isRailwayCheck(MapObject& mapObject)
{
	if (mapObject.railway == "tram")
		return true;
	else
		return false;
}

bool MapManager::isRiverCheck(MapObject& mapObject)
{
	// river will be created manually
	return false;

	if (mapObject.waterway == "river" || mapObject.waterway == "canal")
		return true;
	else
		return false;
}

bool MapManager::isFootwayCheck(MapObject& mapObject)
{
	if (mapObject.highway == "footway" || mapObject.highway == "path")
		return true;
	else
		return false;
}

bool MapManager::isWater(MapObject& mapObject)
{
	if (mapObject.natural == "water")
		return true;
	else
		return false;
}

bool MapManager::isRiverbank(MapObject& mapObject)
{
	if (mapObject.waterway == "riverbank")
		return true;
	else
		return false;
}

bool MapManager::isRamp(MapObject& mapObject)
{
	if (mapObject._custom_object == "_ramp")
		return true;
	else
		return false;
}

void MapManager::selectObject(float X, float Y, bool(MapManager::*objectChecker)(MapObject&))
{
	PointInsidePolygonDetector detector;
	Point selectedPoint(X, Y);

	for (auto& mapObject : mapObjects)
	{
		if(objectChecker == nullptr || objectChecker != nullptr && (this->*objectChecker)(*mapObject))
		{
			if (detector.isInside(mapObject->points, selectedPoint) && mapObject->selectable && !mapObject->isHidden)
			{
				if (mapObject->isSelected)
				{
					mapObject->deselect();
				}
				else
				{
					mapObject->select();
					break;
				}
			}
		}
	}
}

void MapManager::hideObjects()
{
	for (auto& mapObject : mapObjects)
	{
		if (mapObject->isSelected)
			mapObject->isHidden = true;
	}
}

void MapManager::unhideObjects()
{
	for (auto& mapObject : mapObjects)
	{
		mapObject->isHidden = false;
	}
}

void MapManager::deselectObjects()
{
	for (auto& mapObject : mapObjects)
	{
		if(mapObject->isSelected)
			mapObject->deselect();
	}
}

void MapManager::applyMapEditorKeys(Orbit& orbit)
{
	KeyboardManager::Instance()->getKeys();

	if (KeyboardManager::Instance()->checkKey('Z'))
	{
		selectObject(orbit.getFlatCursorX(), orbit.getFlatCursorY());
	}
	if (KeyboardManager::Instance()->checkKey('B'))
	{
		selectObject(orbit.getFlatCursorX(), orbit.getFlatCursorY(), &MapManager::isBuildingCheck);
	}
	if (KeyboardManager::Instance()->checkKey('D'))
	{
		deselectObjects();
	}
	if (KeyboardManager::Instance()->checkKey('H'))
	{
		hideObjects();
	}
	if (KeyboardManager::Instance()->checkKey('U'))
	{
		unhideObjects();
	}
	if (KeyboardManager::Instance()->checkKey('C'))
	{
		if (CameraManager::Instance()->cameraViews.size() > 1)
		{
			currentCameraView++;
			currentCameraView %= CameraManager::Instance()->cameraViews.size();
		}
	}
	if (KeyboardManager::Instance()->checkKey('V'))
	{
		orbit.BirdsEyeView();
	}
	if (KeyboardManager::Instance()->checkKey('S'))
	{
		MapContainer::Instance()->SaveAIPoints();
	}
	if (KeyboardManager::Instance()->checkKey('L'))
	{
		MapContainer::Instance()->LoadAIPoints();
	}

	if (KeyboardManager::Instance()->checkKey('0'))
	{
		addOverlayAttribute("_skip", "yes");
	}
	if (KeyboardManager::Instance()->checkKey('1'))
	{
		addOverlayAttribute("height", "5.00");
	}
	if (KeyboardManager::Instance()->checkKey('2'))
	{
		addOverlayAttribute("height", "10.00");
	}
	if (KeyboardManager::Instance()->checkKey('3'))
	{
		addOverlayAttribute("height", "15.00");
	}
	if (KeyboardManager::Instance()->checkKey('4'))
	{
		addOverlayAttribute("height", "20.00");
	}
	if (KeyboardManager::Instance()->checkKey('5'))
	{
		addOverlayAttribute("height", "25.00");
	}
	if (KeyboardManager::Instance()->checkKey('6'))
	{
		addOverlayAttribute("height", "30.00");
	}
	if (KeyboardManager::Instance()->checkKey('7'))
	{
		addOverlayAttribute("height", "35.00");
	}

	if (KeyboardManager::Instance()->checkKey('F'))
	{
		addOverlayAttribute("roof:shape", "flat");
	}

}

void MapManager::applyOverlays(MapObject& mapObject)
{
	rapidxml::xml_node <>* nodeCeo = overlays.first_node();
	for (rapidxml::xml_node <>* manager = nodeCeo->first_node(); manager; manager = manager->next_sibling()) // (3)
	{
		if (!strcmp(manager->name(), "way"))
		{
			if (std::stoll(manager->first_attribute()->value()) == mapObject.getId())
			{

				for (rapidxml::xml_node <>* a = manager->first_node(); a; a = a->next_sibling())
				{
					std::string currentTag;
					std::string currentTagValue;
					bool skipTag = false;

					if (!strcmp(a->name(), "tag"))
					{
						for (rapidxml::xml_attribute <>* b = a->first_attribute(); b; b = b->next_attribute())
						{
							if (!strcmp(b->name(), "k")) //attribute name
							{
								if (skippedTags.count(b->value()) || temporarySkippedTags.count(b->value()))
								{
									skipTag = true;
									break;
								}
								else if (acceptedTags.count(b->value()))
								{
									currentTag = b->value();
								}
								else
								{
									//assert(!"unknown tag");
								}

							}
							else if (!strcmp(b->name(), "v")) //attribute value
							{
								currentTagValue = b->value();
							}
							else
							{
								break;
							}
						}
						if (!skipTag)
						{
							auto mapElement = tagLongPtrs.find(currentTag);
							if (mapElement != tagLongPtrs.end())
							{
								mapObject.*mapElement->second = 5;
							}
							else
							{
								auto mapElement = tagPtrs.find(currentTag);
								if (mapElement != tagPtrs.end())
								{
									mapObject.*mapElement->second = currentTagValue;
								}
							}
						}
					}
				}



				break;
			}
		}
	}

}

void MapManager::addOverlayAttribute(const char* attribute, const char* value)
{
	char* allocatedValue = overlays.allocate_string(value);

	for (auto& mapObject : mapObjects)
	{
		if (mapObject->isSelected)
		{
			rapidxml::xml_node<>* node = nullptr;

			bool nodeFound = false;
			bool attributeFound = false;
			//find node
			rapidxml::xml_node <>* nodeCeo = overlays.first_node();
			for (rapidxml::xml_node <>* manager = nodeCeo->first_node(); manager; manager = manager->next_sibling()) // (3)
			{
				if (!strcmp(manager->name(), "way"))
				{
					if (std::stoll(manager->first_attribute()->value()) == mapObject->getId())
					{
						nodeFound = true;
						node = manager;


						for (rapidxml::xml_node <>* a = manager->first_node(); a; a = a->next_sibling())
						{
							std::string currentTag;
							std::string currentTagValue;
							bool skipTag = false;

							if (!strcmp(a->name(), "tag"))
							{
								for (rapidxml::xml_attribute <>* b = a->first_attribute(); b; b = b->next_attribute())
								{
									if (!strcmp(b->name(), "k")) //attribute name
									{
										if (!strcmp(b->value(), attribute))
										{
											attributeFound = true;
										}

									}
									else if (!strcmp(b->name(), "v")) //attribute value
									{
										if(attributeFound)
											b->value(allocatedValue);
									}
								}
							}

							if (attributeFound)
								break;
						}

						break;
					}

				}
			}


			if (!nodeFound)
			{

				node = overlays.allocate_node(rapidxml::node_type::node_element, "way");
				char* id = overlays.allocate_string(std::to_string(mapObject->getId()).c_str());
				node->append_attribute(overlays.allocate_attribute("id", id));

				overlays.first_node()->append_node(node);
			}

			if (!attributeFound)
			{
				rapidxml::xml_node<>* subnode = overlays.allocate_node(rapidxml::node_type::node_element, "tag");
				subnode->append_attribute(overlays.allocate_attribute("k", attribute));
				subnode->append_attribute(overlays.allocate_attribute("v", allocatedValue));

				node->append_node(subnode);
			}

		}
	}
}

void MapManager::calculateObjectsFinalGeometry()
{
	for (auto& mapObject : mapObjects)
	{
		mapObject->calculateFinalGeometry();
	}
}

void MapManager::calculateObjectsBoundingCoordinates()
{
	for (auto& mapObject : mapObjects)
	{
		mapObject->calculateBoundingCoordinates();
	}
}

void MapManager::calculateMapBoundingCoordinates()
{
	for (auto& mapObject : mapObjects)
	{
		if (mapObject->minX < minX)
			minX = mapObject->minX;
		if (mapObject->maxX > maxX)
			maxX = mapObject->maxX;
		if (mapObject->minY < minY)
			minY = mapObject->minY;
		if (mapObject->maxY > maxY)
			maxY = mapObject->maxY;
	}
}

void MapManager::generatePolygonsFile()
{
	std::ofstream file;
	file.open("Data/World.txt", std::ios::out);

	for (auto& mapObject : mapObjects)
	{
		for (auto& polygon : mapObject->polygons)
		{
			if (polygon.noOfPoints < 3)
				continue;
			
			file << polygon.idTexture << " ";
			file << polygon.color.red << " " << polygon.color.green << " " << polygon.color.blue << " ";
			file << polygon.additionalColor.red << " " << polygon.additionalColor.green << " " << polygon.additionalColor.blue << " ";
			file << polygon.noOfPoints << " ";

			for (auto& point : polygon.points)
			{
				file << point.x << " " << point.y << " " << point.z << " ";
			}
			for (auto& texturePoint : polygon.texturePoints)
			{
				file << texturePoint.x << " " << texturePoint.y << " " << texturePoint.z << " ";
			}

			file << std::endl;
		}
	}

	file.close();

}
void MapManager::loadPolygonsFromFile()
{
	loadedFromPolygonsFile = true;

	std::ifstream file;
	file.open("Data/World.txt");

	Object3D::Polygon objectPolygon;
	file >> objectPolygon.idTexture;
	
	do
	{
		Object3D newWorldObject;

		file >> objectPolygon.color.red;
		file >> objectPolygon.color.green;
		file >> objectPolygon.color.blue;

		file >> objectPolygon.additionalColor.red;
		file >> objectPolygon.additionalColor.green;
		file >> objectPolygon.additionalColor.blue;

		file >> objectPolygon.noOfPoints;

		for (int q = 0; q < objectPolygon.noOfPoints; q++)
		{
			Point newPoint;
			file >> newPoint.x >> newPoint.y >> newPoint.z;
			objectPolygon.points.push_back(std::move(newPoint));
		}

		for (int q = 0; q < objectPolygon.noOfPoints; q++)
		{
			Point newTexturePoint;
			file >> newTexturePoint.x >> newTexturePoint.y >> newTexturePoint.z;
			objectPolygon.texturePoints.push_back(std::move(newTexturePoint));
		}

		newWorldObject.polygons.push_back(objectPolygon);
		polygonsObjects.push_back(std::make_unique<Object3D>(newWorldObject));
		objectPolygon.points.clear();
		objectPolygon.texturePoints.clear();

	} while (file >> objectPolygon.idTexture);

	file.close();
}

void MapManager::saveExtraObjects()
{
	//nodes
	for (auto& extraNode : extraNodes)
	{
		int nodeObjectPosition = 0;
		MapObject* nodeObject = nullptr;

		for (auto& extraObject : MapContainer::Instance()->extraObjects)
		{
			if (extraNode.first == extraObject->getId())
			{
				nodeObject = extraObject.get();
				break;
			}
			nodeObjectPosition++;
		}

		auto newNode = extraObjects.allocate_node(rapidxml::node_type::node_element, "node");
		char* id = extraObjects.allocate_string(std::to_string(extraNode.first).c_str());
		newNode->append_attribute(extraObjects.allocate_attribute("id", id));
		char* lat = extraObjects.allocate_string(std::to_string(extraNode.second.lat).c_str());
		newNode->append_attribute(extraObjects.allocate_attribute("lat", lat));
		char* lon = extraObjects.allocate_string(std::to_string(extraNode.second.lon).c_str());
		newNode->append_attribute(extraObjects.allocate_attribute("lon", lon));

		if (nodeObject != nullptr)
		{
			rapidxml::xml_node<>* subnode = extraObjects.allocate_node(rapidxml::node_type::node_element, "tag");

			for (auto& attribute : nodeObject->getObjectXMLTags())
			{
				subnode->append_attribute(extraObjects.allocate_attribute(attribute.first, attribute.second));
			}

			newNode->append_node(subnode);
			MapContainer::Instance()->extraObjects.erase(MapContainer::Instance()->extraObjects.begin() + nodeObjectPosition);
		}

		extraObjects.first_node()->append_node(newNode);
	}

	//objects
	for (auto& extraObject : MapContainer::Instance()->extraObjects)
	{

		auto newWay = extraObjects.allocate_node(rapidxml::node_type::node_element, "way");
		char* id = extraObjects.allocate_string(std::to_string(extraObject->getId()).c_str());
		newWay->append_attribute(extraObjects.allocate_attribute("id", id));

		for (auto& ref : extraObject.get()->refs)
		{
			rapidxml::xml_node<>* subnode = extraObjects.allocate_node(rapidxml::node_type::node_element, "nd");

			char* refValue = extraObjects.allocate_string(std::to_string(ref).c_str());
			subnode->append_attribute(extraObjects.allocate_attribute("ref", refValue));

			newWay->append_node(subnode);
		}

		//main tag
		{
			rapidxml::xml_node<>* subnode = extraObjects.allocate_node(rapidxml::node_type::node_element, "tag");
			for (auto& attribute : extraObject->getObjectXMLTags())
			{
				subnode->append_attribute(extraObjects.allocate_attribute(attribute.first, attribute.second));
			}
			newWay->append_node(subnode);
		}

		//addtional tags
		{
			for (auto& tag : extraObject->getObjectXMLExtraTags())
			{
				rapidxml::xml_node<>* subnode = extraObjects.allocate_node(rapidxml::node_type::node_element, "tag");
				for (auto& attribute : tag)
				{
					char* value = extraObjects.allocate_string(attribute.second.c_str());
					subnode->append_attribute(extraObjects.allocate_attribute(attribute.first, value));
				}
				newWay->append_node(subnode);
			}
		}

		extraObjects.first_node()->append_node(newWay);
	}


	//saving
	std::filebuf fb;
	fb.open("extraObjects.xml", std::ios::out);
	std::ostream os(&fb);
	rapidxml::print(os, extraObjects);
	fb.close();

	extraNodes.clear();
	MapContainer::Instance()->extraObjects.clear();
	extraObjects.clear();


	rereadMap();
}