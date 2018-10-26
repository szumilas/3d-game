#include <fstream>
#include <iostream>
#include <string>

#include "MapManager.h"

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
	//auto fileContent = fileToCharReader("xml.txt");
	auto fileContent = fileToCharReader(fileName);

	try
	{
		document.parse < 0 >(fileContent.get()); // (2)
	}
	catch (const rapidxml::parse_error & e)
	{
		//TODO: errors
	}

	//rapidxml::print(std::cout, document, 0);

	createNodesMap();
	createMapObjectsArray();
	
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
			MapObject mapObject;
			long long id = std::stoll(manager->first_attribute()->value());

			for (rapidxml::xml_node <>* a = manager->first_node(); a; a = a->next_sibling())
			{
				std::string currentTag;
				std::string currentTagValue;
				bool skipTag = false;

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
							auto mapElement = tagStringPtrs.find(currentTag);
							if (mapElement != tagStringPtrs.end())
							{
								mapObject.*mapElement->second = currentTagValue;
							}
						}
					}
				}
			}

			if (!mapObject.height.empty())
			{
				mapObject._height = std::stof(mapObject.height);
			}
			else
			{
				mapObject._height = 15.0;
			}
			if (!mapObject.min_height.empty())
			{
				mapObject._min_height = std::stof(mapObject.min_height);
			}
			else
			{
				mapObject._min_height = 0.0;
			}

			if (id == 33286825)
			{
				//mapObjects.push_back(std::make_unique<HighlightedObject>(mapObject));
			}
			else if (!mapObject.area_highway.empty())
			{
				mapObjects.push_back(std::make_unique<Street>(mapObject));
			}
			else if (!mapObject.building.empty() || !mapObject.building_part.empty())
			{
				mapObjects.push_back(std::make_unique<Building>(mapObject));
			}
			else if (mapObject.landuse == "grass" || mapObject.landuse == "village_green")
			{
				mapObjects.push_back(std::make_unique<GreenArea>(mapObject));
			}
			else if (mapObject.waterway == "river" || mapObject.waterway == "canal")
			{
				mapObjects.push_back(std::make_unique<River>(mapObject));
			}
			else if (!mapObject.barrier.empty())
			{
				mapObjects.push_back(std::make_unique<Barrier>(mapObject));
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

	for (auto& mapObject : mapObjects)
	{
		for (auto& ref : mapObject->refs)
		{
			Point newPoint;
			newPoint.x = nodes.at(ref).posX;
			newPoint.y = nodes.at(ref).posY;
			mapObject->points.push_back(newPoint);
		}
	}
}