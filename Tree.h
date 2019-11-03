#pragma once

#include "MapObject.h"
#include <array>

class Tree : public MapObject
{
public:

	Tree(long long ref);

	//void display();
	void calculateXYfromRef(const std::map<long long, node> &nodes);
	std::vector<std::pair<char*, char*>> getObjectXMLTags();

private:

	std::array<Point, 12> treeCrownPoints;
};