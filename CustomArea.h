#pragma once

#include "MapObject.h"

class CustomArea : public MapObject
{
public:

	CustomArea(MapObject& mapObject);

	void calculateFinalGeometry();

	std::vector<std::pair<char*, char*>> getObjectXMLTags();
	std::vector<std::vector<std::pair<char*, std::string>>> getObjectXMLExtraTags();

private:


public:


private:

	unsigned int idTexture;

};