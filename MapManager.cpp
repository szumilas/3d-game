#include <fstream>
#include <iostream>
#include <string>
#include "PointInsidePolygonDetector.h"

#include "MapManager.h"
#include "Skybox.h"
#include "RelationalMapObject.h"

#include "rapidxml_print.hpp"

void printAttributes(rapidxml::xml_node <>* node)
{
	for (rapidxml::xml_attribute <>* a = node->first_attribute(); a; a = a->next_attribute())
	{
		std::cout << a->name() << "' = '" << a->value() << "'\t";
	}
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

void MapManager::readMap(const char * fileName)
{
	auto fileContent = fileToCharReader(fileName);

	try
	{
		document.parse < 0 >(fileContent.get()); // (2)
	}
	catch (const rapidxml::parse_error & e)
	{
		//TODO: errors
	}
	
	overlayContent = fileToCharReader("worldOverlays.xml");

	try
	{
		overlays.parse < 0 >(overlayContent.get()); // (2)
	}
	catch (const rapidxml::parse_error & e)
	{
		//TODO: errors
	}

	
	createNodesMap();
	createMapObjectsArray();
	calculateNodesPositions();
	removeSkippedObjects();

	
	calculateObjectsFinalGeometry();
	calculateObjectsBoundingCoordinates();
	calculateMapBoundingCoordinates();

	mapObjects.push_back(std::make_unique<Skybox>(Skybox(textureManager, minX, maxX, minY, maxY)));
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
	rapidxml::xml_node <>* nodeCeo = document.first_node();
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

void MapManager::createMapObjectsArray()
{
	rapidxml::xml_node <>* nodeCeo = document.first_node();
	for (rapidxml::xml_node <>* manager = nodeCeo->first_node(); manager; manager = manager->next_sibling()) // (3)
	{
		if (!strcmp(manager->name(), "way"))
		{
			MapObject mapObject(std::stoll(manager->first_attribute()->value()));

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
				if ((this->*check.first)(mapObject))
				{
					(this->*check.second)(mapObject);
					break;
				}
			}
		}
	}
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
				if ((this->*check.first)(relationalMapObject))
				{
					(this->*check.second)(relationalMapObject);
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

void MapManager::calculateNodesPositions()
{
	double minLat = 51.112663;
	double minLon = 17.056106;

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

void MapManager::selectObject(float X, float Y, bool(MapManager::*objectChecker)(MapObject&))
{
	PointInsidePolygonDetector detector;
	Point selectedPoint(X, Y);

	for (auto& mapObject : mapObjects)
	{
		if(objectChecker == nullptr || objectChecker != nullptr && (this->*objectChecker)(*mapObject))
		{
			if (detector.isInside(mapObject->points, selectedPoint))
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
	std::vector<bool> keys(200);
	for (int q = 0; q < keys.size(); q++)
		keys[q] = GetAsyncKeyState(q);

	if (keys[static_cast<int>('Z')])
	{
		selectObject(orbit.getFlatCursorX(), orbit.getFlatCursorY());
	}
	if (keys[static_cast<int>('B')])
	{
		selectObject(orbit.getFlatCursorX(), orbit.getFlatCursorY(), &MapManager::isBuildingCheck);
	}
	if (keys[static_cast<int>('D')])
	{
		deselectObjects();
	}
	if (keys[static_cast<int>('H')])
	{
		hideObjects();
	}
	if (keys[static_cast<int>('U')])
	{
		unhideObjects();
	}

	if (keys[static_cast<int>('0')])
	{
		addOverlayAttribute("_skip", "yes");
	}
	if (keys[static_cast<int>('1')])
	{
		addOverlayAttribute("height", "5.00");
	}
	if (keys[static_cast<int>('2')])
	{
		addOverlayAttribute("height", "10.00");
	}
	if (keys[static_cast<int>('3')])
	{
		addOverlayAttribute("height", "15.00");
	}
	if (keys[static_cast<int>('4')])
	{
		addOverlayAttribute("height", "20.00");
	}
	if (keys[static_cast<int>('5')])
	{
		addOverlayAttribute("height", "25.00");
	}
	if (keys[static_cast<int>('6')])
	{
		addOverlayAttribute("height", "30.00");
	}
	if (keys[static_cast<int>('7')])
	{
		addOverlayAttribute("height", "35.00");
	}

	if (keys[static_cast<int>('F')])
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
											b->value(value);
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
				subnode->append_attribute(overlays.allocate_attribute("v", value));

				node->append_node(subnode);
			}

		}
	}
}

void MapManager::calculateObjectsFinalGeometry()
{
	for (auto& mapObject : mapObjects)
	{
		mapObject->calculateFinalGeometry(textureManager);
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