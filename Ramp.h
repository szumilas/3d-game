#pragma once

#include "MapObject.h"

class Ramp : public MapObject
{
public:
	Ramp(MapObject& mapObject);

	void calculateFinalGeometry();
	void recalculateFinalGeometry();
	virtual void calculateXYfromRef(const std::map<long long, node> &nodes);
	std::vector<std::pair<char*, char*>> getObjectXMLTags();
	std::vector<std::vector<std::pair<char*, std::string>>> getObjectXMLExtraTags();

private:


public:


private:

	std::vector<Point> originalPoints;

};