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

	createNodesArray();
	createBuildingsArray();
	/*rapidxml::xml_node <>* nodeCeo = document.first_node();
	for (rapidxml::xml_node <>* manager = nodeCeo->first_node(); manager; manager = manager->next_sibling()) // (3)
	{
		/*std::cout << "Manager: '" << manager->name() << "', known as:\t";
		printAttributes(manager);
		std::cout << "\tfor:\n";

		for (rapidxml::xml_node <>* realWorker = manager->first_node(); realWorker; realWorker = realWorker->next_sibling())
		{
			std::cout << "\t'" << realWorker->name() << "' id=" << realWorker->first_attribute()->value() << " working on: '" << realWorker->value() << "'\n";
		}*/

		/*if (!strcmp(manager->name(), "node"))
		{
			printAttributes(manager);
		}
	}*/
}

void MapManager::createNodesArray()
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
				nodes.push_back(newNode);
			}
			std::cout << std::endl;
		}
	}

}


void MapManager::createBuildingsArray()
{
	rapidxml::xml_node <>* nodeCeo = document.first_node();
	for (rapidxml::xml_node <>* manager = nodeCeo->first_node(); manager; manager = manager->next_sibling()) // (3)
	{
		if (!strcmp(manager->name(), "way"))
		{
			std::vector<long long> refs;
			bool isBuilding = false;

			for (rapidxml::xml_node <>* a = manager->first_node(); a; a = a->next_sibling())
			{
				if (!strcmp(a->name(), "nd"))
				{
					for (rapidxml::xml_attribute <>* b = a->first_attribute(); b; b = b->next_attribute())
					{
						if (!strcmp(b->name(), "ref"))
						{
							refs.push_back(std::stoll(b->value()));
							break;
						}
					}
				}
				else if (!strcmp(a->name(), "tag"))
				{
					for (rapidxml::xml_attribute <>* b = a->first_attribute(); b; b = b->next_attribute())
					{
						if (!strcmp(b->name(), "k"))
						{
							if (!strcmp(b->value(), "building"))
							{
								isBuilding = true;
								break;
							}
						}
					}
				}
			}

			if (isBuilding)
			{
				buildings.push_back(refs);
			}

		}
	}

}

void MapManager::calculateNodesPositions()
{
	double minLat = 1000;
	double minLon = 1000;

	for (auto& node : nodes)
	{
		if (node.lat < minLat)
		{
			minLat = node.lat;
		}
		if (node.lon < minLon)
		{
			minLon = node.lon;
		}
	}

	for (auto& node : nodes)
	{
		node.posX = static_cast<float>((node.lon - minLon) * longituteRatio);
		node.posY = static_cast<float>((node.lat - minLat) * latitudeRatio);
	}

}