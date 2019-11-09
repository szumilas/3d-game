#pragma once

#include "MapObject.h"

class Ramp : public MapObject
{
public:
	Ramp(MapObject& mapObject);

	void calculateFinalGeometry();
	void recalculateFinalGeometry();
	std::vector<std::pair<char*, char*>> getObjectXMLTags();
	std::vector<std::vector<std::pair<char*, std::string>>> getObjectXMLExtraTags();

private:


public:


private:


};