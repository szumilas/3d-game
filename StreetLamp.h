#pragma once

#include "MapObject.h"

class StreetLamp : public MapObject
{
public:

	StreetLamp(long long ref);

	void calculateXYfromRef(const std::map<long long, node> &nodes);
	std::vector<std::pair<char*, char*>> getObjectXMLTags();

private:
	float size;

};